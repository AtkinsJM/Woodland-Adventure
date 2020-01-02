// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceManager.h"
#include "Checkpoint.h"

ARaceManager::ARaceManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NextCheckpointIndex = 0;
	bIsRaceActive = false;
}

void ARaceManager::BeginPlay()
{
	Super::BeginPlay();
	for (ACheckpoint* Checkpoint : Checkpoints)
	{
		Checkpoint->SetRaceManager(this);
	}

}

void ARaceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Ticking"));
	if (bIsRaceActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Time remaining reduced"));
		TimeRemaining -= DeltaTime;
	}
}

void ARaceManager::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ARaceManager::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ARaceManager::Interact()
{
	Super::Interact();
	StartRace();
}

void ARaceManager::StartRace()
{
	UE_LOG(LogTemp, Warning, TEXT("Race started"));
	TimeRemaining = 10.0f;
	bIsRaceActive = true;
	NextCheckpointIndex = 0;
	SetupNextCheckpoint();

}

void ARaceManager::EndRace()
{
	bIsRaceActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Race finished"));
}

void ARaceManager::SetupNextCheckpoint()
{
	ACheckpoint* NextCheckpoint = Checkpoints[NextCheckpointIndex];
	if (NextCheckpoint)
	{
		NextCheckpoint->SetupCheckpoint();
	}
}

void ARaceManager::PassCurrentCheckpoint()
{
	UE_LOG(LogTemp, Warning, TEXT("Passed checkpoint %i"), NextCheckpointIndex);
	NextCheckpointIndex++;
	if (NextCheckpointIndex < Checkpoints.Num())
	{
		TimeRemaining += 10.0f;
		SetupNextCheckpoint();
	}
	else
	{
		EndRace();
	}
}
