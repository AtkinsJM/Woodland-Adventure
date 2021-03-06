// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "RaceManager.generated.h"

/**
 * 
 */
UCLASS()
class WOODLANDADVENTURE_API ARaceManager : public AInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARaceManager();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Race")
	TArray<class ACheckpoint*> Checkpoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Race")
	int32 NumberOfLaps;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Race")
	float StartTime;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Race")
	float CheckpointBonusTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	class USoundCue* CountdownSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	class USoundCue* TimerSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundCue* WinSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundCue* LoseSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundCue* PassCheckpointSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class UAudioComponent* AudioComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	virtual void Interact(class AAnimalCharacter* InteractingCharacter) override;

	void StartCountdown();

	void StartRace();

	void EndRace();

	void SetupNextCheckpoint();

	void PassCurrentCheckpoint();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentLap() { return CurrentLap; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCheckpointsPassed() { return NextCheckpointIndex; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetNumberOfCheckpoints() { return Checkpoints.Num(); }

	UFUNCTION(BlueprintCallable)
	bool IsRaceActive() { return bIsRaceActive; }

	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() { return TimeRemaining; }

private:
	int32 NextCheckpointIndex;
	bool bIsRaceActive;
	bool bIsTimerActive;

	float TimeRemaining;

	int32 CurrentLap;

	FTimerHandle CountdownTimerHandle;

	AAnimalCharacter* RacingCharacter;
};