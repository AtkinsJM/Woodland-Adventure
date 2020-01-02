// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceManager.h"
#include "Checkpoint.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ARaceManager::ARaceManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(GetRootComponent());

	NextCheckpointIndex = 0;
	bIsRaceActive = false;

	StartTime = 10.0f;
	CheckpointBonusTime = 10.0f;
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
	if (bIsRaceActive)
	{
		TimeRemaining -= DeltaTime;
		if (TimeRemaining < 0.0f) 
		{ 
			TimeRemaining = 0.0f; 
			EndRace();
		}
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
	bIsRaceActive = true;
	TimeRemaining = StartTime;
	NextCheckpointIndex = 0;
	SetupNextCheckpoint();
	if (AudioComponent && TimerSound)
	{
		AudioComponent->SetSound(TimerSound);
		AudioComponent->Play();
	}
}

void ARaceManager::EndRace()
{
	bIsRaceActive = false;
	if (TimeRemaining > 0.0f)
	{
		//WIN!
		UE_LOG(LogTemp, Warning, TEXT("You won the race!"));
	}
	else
	{
		//LOSE!
		UE_LOG(LogTemp, Warning, TEXT("You lost the race!"));
	}
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
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
	NextCheckpointIndex++;
	if (NextCheckpointIndex < Checkpoints.Num())
	{
		TimeRemaining += CheckpointBonusTime;
		SetupNextCheckpoint();
	}
	else
	{
		EndRace();
	}
}
