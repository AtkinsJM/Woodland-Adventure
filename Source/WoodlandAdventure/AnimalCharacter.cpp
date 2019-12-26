// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Interactable.h"

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

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	// Attach camera to end of boom and let boom control its rotation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bIsSleeping = false;
	bCanMove = true;

	SleepLength = 5.0f;

	InteractingActor = nullptr;
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
	bIsSleeping = true;
	GetWorld()->GetTimerManager().SetTimer(SleepTimerHandle, this, &AAnimalCharacter::EndSleep, SleepLength, true);
}

void AAnimalCharacter::EndSleep()
{
	bIsSleeping = false;
}

void AAnimalCharacter::Interact()
{
	if (InteractingActor)
	{
		InteractingActor->Interact();
	}
}

void AAnimalCharacter::ZoomCamera(float Value)
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength += (ZoomRate * Value), DefaultCameraBoomLength * MinZoom, DefaultCameraBoomLength * MaxZoom);
}

void AAnimalCharacter::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AInteractable* Interactable = Cast<AInteractable>(OtherActor);
		if (Interactable && !InteractingActor)
		{
			InteractingActor = Interactable;
		}
	}
}

void AAnimalCharacter::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AInteractable* Interactable = Cast<AInteractable>(OtherActor);
		if (Interactable && Interactable == InteractingActor)
		{
			InteractingActor = nullptr;
		}
	}
}

