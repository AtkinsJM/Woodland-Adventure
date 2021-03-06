// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Tree.generated.h"

/**
 * 
 */
UCLASS()
class WOODLANDADVENTURE_API ATree : public AInteractable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATree();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	class UBoxComponent* CanopyVolume;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<AActor> AppleAsset;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	int32 MaxApplesToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class AAnimalCharacter* InteractingCharacter) override;

	FVector GetSpawnPoint();

	void SpawnApple(FVector Location);

	bool IsOccupied(FVector Location, float Radius);

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:


};
