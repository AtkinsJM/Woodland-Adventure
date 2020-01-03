// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "AnimalCharacter.h"
#include "RaceManager.h"
#include "Components/WidgetComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(Root);

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Volumee"));
	InteractionVolume->SetupAttachment(Root);
	// TODO: consider removing, as these collision responses don't seem to actually do anything...
	InteractionVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	InteractionVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);

	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &ACheckpoint::OnEndOverlap);

	Marker = CreateDefaultSubobject<UWidgetComponent>(TEXT("Marker"));
	Marker->SetupAttachment(GetRootComponent());
	Marker->SetWidgetSpace(EWidgetSpace::World);
	Marker->SetDrawSize(FVector2D(30.0f, 30.0f));
	Marker->SetVisibility(false);

	bIsNextCheckpoint = false;
	bIsPassed = false;

	RaceManager = nullptr;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Marker)
	{
		FRotator WantedRotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();
		WantedRotation.Pitch *= -1;
		WantedRotation.Yaw += 180;
		Marker->SetWorldRotation(WantedRotation);
	}
}

void ACheckpoint::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		//TODO check character is correct (use required character enum or just hard-code pig?)
		if (Cast<AAnimalCharacter>(OtherActor) && bIsNextCheckpoint && !bIsPassed && RaceManager && RaceManager->IsRaceActive())
		{
			PassCheckpoint();
		}
	}
}

void ACheckpoint::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

void ACheckpoint::PassCheckpoint()
{
	bIsPassed = true;
	bIsNextCheckpoint = false;
	if (Marker)
	{
		Marker->SetVisibility(false);
	}
	if (RaceManager)
	{
		RaceManager->PassCurrentCheckpoint();
	}
}

void ACheckpoint::SetupCheckpoint()
{
	bIsNextCheckpoint = true;
	bIsPassed = false;
	if (Marker)
	{
		Marker->SetVisibility(true);
	}
}
