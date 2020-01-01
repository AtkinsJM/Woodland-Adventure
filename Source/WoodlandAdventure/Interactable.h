// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimalType.h"
#include "Interactable.generated.h"

UCLASS()
class WOODLANDADVENTURE_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	class USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	class USphereComponent* InteractionSphere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float InteractionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	FString InteractionPrompt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	class USoundCue* InteractionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	bool bPlaySoundOnInteraction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TArray<EAnimalType> RequiredAnimalTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bIsInteractable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 NumberOfInteractions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TArray<class UStaticMeshComponent*> MeshesToHighlight;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Interact();

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsRequiredAnimal(EAnimalType AnimalType);

	void ToggleHighlight(bool Value);
};
