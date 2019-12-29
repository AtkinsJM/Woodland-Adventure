// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AnimalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WOODLANDADVENTURE_API AAnimalPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAnimalPlayerController();

	class UAnimalCharacterInputComponent* AnimalCharacterInputComponent;

	class AAnimalCharacter* AnimalCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	TArray<TSubclassOf<AAnimalCharacter>> CharacterClasses;

	TArray<AAnimalCharacter*> PlayableCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

	/** Base rotation rates to scale rotation functions for the camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseCameraTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseCameraLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	bool bInvertYAxis;
		
	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator InitialRotation;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurnInterpSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* Pawn) override;

	virtual void OnUnPossess() override;

public:
	
	void HandleMovement(float DeltaTime);
	void MoveForward(float Value);
	void MoveSideways(float Value);
	
	void LookUp(float Value);
	void Turn(float Value);

	void ZoomCamera(float Value);

	void Sleep();

	void Interact();

	void PossessCharacter();

	void FindPlayableCharacters();

	UFUNCTION()
	void CycleThroughCharacters(int32 Value);

	UFUNCTION(BlueprintCallable)
	AAnimalCharacter* GetPlayableCharacterAtIndex(int32 Index);
	
	void SwitchCharacter();

private:
	bool bIsControllingCameraOnly;
	
	FVector2D MovementDirection;

	int PossessedCharacterIndex;
};
