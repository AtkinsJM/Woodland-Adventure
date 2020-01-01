// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Interactable.h"
#include "Tree.h"
#include "Kismet/KismetMathLibrary.h"
#include "Apple.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAnimalCharacter::AAnimalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Volume"));
	InteractionVolume->SetupAttachment(GetRootComponent());

	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AAnimalCharacter::OnBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AAnimalCharacter::OnEndOverlap);
	
	DefaultCameraBoomLength = 300.0f;

	MaxZoom = 2.0f;
	MinZoom = 0.5f;
	ZoomRate = 5.0f;

	// Create Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	// Disabled because camera lag was annoying when possessing characters
	//CameraBoom->bEnableCameraLag = true;
	//CameraBoom->bEnableCameraRotationLag = true;

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	// Attach camera to end of boom and let boom control its rotation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bIsSleeping = false;
	bCanMove = true;
	bIsShakingTree = false;
	bIsEating = false;

	SleepLength = 5.0f;
	EatLength = 1.0f;

	InteractableActor = nullptr;
	PossessableCharacter = nullptr;

	Icon = nullptr;

	NumApplesEaten = 0;

	SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();
	if (SkeletalMesh)
	{
		SkeletalMesh->SetCustomDepthStencilValue(1);
	}
}

// Called when the game starts or when spawned
void AAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	CameraBoom->TargetArmLength = DefaultCameraBoomLength;
}

// Called every frame
void AAnimalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAnimalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAnimalCharacter::StartSleep()
{
	if (!bCanMove || bIsSleeping) { return; }
	bIsSleeping = true;
	GetWorld()->GetTimerManager().SetTimer(SleepTimerHandle, this, &AAnimalCharacter::EndSleep, SleepLength, false);
}

void AAnimalCharacter::EndSleep()
{
	bIsSleeping = false;
}

void AAnimalCharacter::Interact()
{
	if (InteractableActor)
	{		
		InteractableActor->ToggleHighlight(false);
		// TODO refine this (use required animals list on interactable?
		if (AnimalType == EAnimalType::EAT_Stag && Cast<ATree>(InteractableActor))
		{
			StartShakingTree();
		}
		else if (AnimalType == EAnimalType::EAT_Pig && Cast<AApple>(InteractableActor))
		{
			StartEating();
		}
		else
		{
			InteractableActor->Interact();
		}
	}
}

void AAnimalCharacter::StartShakingTree()
{
	if (!InteractableActor) { return; }
	bCanMove = false;
	bIsShakingTree = true;
	LookAt(InteractableActor);
}

void AAnimalCharacter::EndShakingTree()
{
	bIsShakingTree = false;
	bCanMove = true;
}

void AAnimalCharacter::StartEating()
{
	if (!InteractableActor) { return; }


	bCanMove = false;
	bIsEating = true;

	LookAt(InteractableActor);
	   
	if (PigEatSound)
	{
		UGameplayStatics::PlaySound2D(this, PigEatSound);
	}

	GetWorld()->GetTimerManager().SetTimer(EatTimerHandle, this, &AAnimalCharacter::EndEating, EatLength, false);
}

void AAnimalCharacter::EndEating()
{
	bIsEating = false;
	bCanMove = true;
	NumApplesEaten++;
	InteractableActor = nullptr;
}

void AAnimalCharacter::LookAt(AActor* OtherActor)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OtherActor->GetActorLocation());
	FRotator WantedRotation = GetActorRotation();
	WantedRotation.Yaw = LookAtRotation.Yaw;
	SetActorRotation(WantedRotation);
}

void AAnimalCharacter::ZoomCamera(float Value)
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength += (ZoomRate * Value), DefaultCameraBoomLength * MinZoom, DefaultCameraBoomLength * MaxZoom);
}

void AAnimalCharacter::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// TODO: find a better method of avoiding interactable changing whilst interacting with it
	if (bIsEating || bIsSleeping || bIsShakingTree) { return; }
	if (OtherActor && OtherActor != this)
	{
		AInteractable* Interactable = Cast<AInteractable>(OtherActor);
		if (Interactable && Interactable->IsRequiredAnimal(AnimalType) && Interactable->bIsInteractable && !InteractableActor)
		{
			InteractableActor = Interactable;
			InteractableActor->ToggleHighlight(true);
			return;
		}
		AAnimalCharacter* Possessable = Cast<AAnimalCharacter>(OtherActor);
		if (Possessable && !PossessableCharacter)
		{
			PossessableCharacter = Possessable;
			PossessableCharacter->ToggleHighlight(true);
			return;
		}
	}
}

void AAnimalCharacter::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (bIsEating || bIsSleeping || bIsShakingTree) { return; }
	if (OtherActor && OtherActor != this)
	{
		AInteractable* Interactable = Cast<AInteractable>(OtherActor);
		if (Interactable && Interactable == InteractableActor)
		{
			InteractableActor->ToggleHighlight(false);
			InteractableActor = nullptr;
			return;
		}
		AAnimalCharacter* Possessable = Cast<AAnimalCharacter>(OtherActor);
		if (Possessable && Possessable == PossessableCharacter)
		{
			PossessableCharacter->ToggleHighlight(false);
			PossessableCharacter = nullptr;
			return;
		}
	}
}

void AAnimalCharacter::ToggleHighlight(bool Value)
{
	if (!SkeletalMesh) { return; }
	SkeletalMesh->SetRenderCustomDepth(Value);
}

