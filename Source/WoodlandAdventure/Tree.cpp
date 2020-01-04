// Fill out your copyright notice in the Description page of Project Settings.

#include "Tree.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AnimalCharacter.h"

// Fill out your copyright notice in the Description page of Project Settings.

// Sets default values
ATree::ATree()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CanopyVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Canopy Volume"));
	CanopyVolume->SetupAttachment(GetRootComponent());

	MaxApplesToSpawn = 3;
}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATree::Interact(class AAnimalCharacter* InteractingCharacter)
{
	Super::Interact(InteractingCharacter);

	if (AppleAsset)
	{
		//Get random number
		int32 NumApplesToSpawn = FMath::RandRange(0, MaxApplesToSpawn);
		//Try spawn that many apples
		for (int i = 0; i < NumApplesToSpawn; i++)
		{			
			FVector SpawnPoint;
			do
			{
				SpawnPoint = GetSpawnPoint();
			} while (IsOccupied(SpawnPoint, 0.10f));
			
			////Get location in spawn volume
			
			////Check unoccupied

			////Repeat until all apples spawned
			SpawnApple(SpawnPoint);
		}
		
	}
}


FVector ATree::GetSpawnPoint()
{
	FVector BoxExtent = CanopyVolume->GetScaledBoxExtent();
	FVector BoxOrigin = CanopyVolume->GetComponentLocation();
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent);
	return Point;
}

bool ATree::IsOccupied(FVector Location, float Radius)
{
	FHitResult HitResult;
	bool bHasHit = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	return bHasHit;
}

void ATree::SpawnApple(FVector Location)
{
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AActor>(AppleAsset, Location, FRotator(0.0f), SpawnParams);
}


void ATree::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ATree::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

