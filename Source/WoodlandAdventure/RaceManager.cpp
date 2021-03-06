// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceManager.h"
#include "Checkpoint.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "AnimalCharacter.h"

ARaceManager::ARaceManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(GetRootComponent());

	NextCheckpointIndex = 0;
	bIsRaceActive = false;
	bIsTimerActive = false;

	StartTime = 10.0f;
	CheckpointBonusTime = 10.0f;

	NumberOfLaps = 1;
	CurrentLap = 1;

	RacingCharacter = nullptr;
}

void ARaceManager::BeginPlay()
{
	Super::BeginPlay();
	for (ACheckpoint* Checkpoint : Checkpoints)
	{
		Checkpoint->SetRaceManager(this);
		Checkpoint->SetActorHiddenInGame(true);
	}

}

void ARaceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsRaceActive && bIsTimerActive)
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

void ARaceManager::Interact(class AAnimalCharacter* InteractingCharacter)
{
	Super::Interact(InteractingCharacter);
	RacingCharacter = InteractingCharacter;
	StartCountdown();
}

void ARaceManager::StartCountdown()
{
	for (ACheckpoint* Checkpoint : Checkpoints)
	{
		Checkpoint->SetActorHiddenInGame(false);
	}
	bIsRaceActive = true;
	TimeRemaining = StartTime;
	NextCheckpointIndex = 0;
	CurrentLap = 1;
	SetupNextCheckpoint();
		
	if (RacingCharacter)
	{
		//TODO: make character face correct direction (same rotation as startpoint? towards next checkpoint?)
		RacingCharacter->SetCanMove(false);
	}

	if (CountdownSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), CountdownSound);
	}

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ARaceManager::StartRace, 3.0f, false);
}

void ARaceManager::StartRace()
{
	bIsTimerActive = true;
	
	if (RacingCharacter)
	{
		RacingCharacter->SetCanMove(true);
	}

	if (AudioComponent && TimerSound)
	{
		AudioComponent->SetSound(TimerSound);
		AudioComponent->Play();
	}
}

void ARaceManager::EndRace()
{
	bIsRaceActive = false;
	bIsTimerActive = false;
	for (ACheckpoint* Checkpoint : Checkpoints)
	{
		Checkpoint->SetActorHiddenInGame(true);
	}	
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
	if (TimeRemaining > 0.0f)
	{
		if (WinSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), WinSound);
		}
	}
	else
	{
		if (LoseSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), LoseSound);
		}
	}
	RacingCharacter = nullptr;
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
	// Increment checkpoint number
	NextCheckpointIndex++;
	// If exceeded number of checkpoints
	if (NextCheckpointIndex == Checkpoints.Num())
	{
		// If final lap, end game; else increment lap number and reset checkpoint number
		if (CurrentLap == NumberOfLaps)
		{
			EndRace();
			return;
		}
		else
		{
			NextCheckpointIndex = 0;
			CurrentLap++;
		}
	}
	
	TimeRemaining += CheckpointBonusTime;
	SetupNextCheckpoint();
	if (PassCheckpointSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PassCheckpointSound);
	}
	
}
