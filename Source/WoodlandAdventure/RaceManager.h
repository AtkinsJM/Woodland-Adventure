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

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Checkpoints")
	TArray<class ACheckpoint*> Checkpoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	virtual void Interact() override;

	void StartRace();

	void EndRace();

	void SetupNextCheckpoint();

	void PassCurrentCheckpoint();

	UFUNCTION(BlueprintCallable)
	bool IsRaceActive() { return bIsRaceActive; }

	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() { return TimeRemaining; }

private:
	int32 NextCheckpointIndex;
	bool bIsRaceActive;

	float TimeRemaining;
};