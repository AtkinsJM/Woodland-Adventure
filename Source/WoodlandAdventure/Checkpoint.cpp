// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "AnimalCharacter.h"
#include "RaceManager.h"

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

}

void ACheckpoint::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		//TODO check character is correct (use required character enum or just hard-code pig?)
		if (Cast<AAnimalCharacter>(OtherActor) && bIsNextCheckpoint && !bIsPassed)
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
	if (RaceManager)
	{
		RaceManager->PassCurrentCheckpoint();
	}
}

void ACheckpoint::SetupCheckpoint()
{
	bIsNextCheckpoint = true;
	bIsPassed = false;
}
