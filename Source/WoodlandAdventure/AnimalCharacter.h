// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnimalType.h"
#include "AnimalCharacter.generated.h"


UCLASS()
class WOODLANDADVENTURE_API AAnimalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimalCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	EAnimalType AnimalType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	class USphereComponent* InteractionVolume;

	/** Camera boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float DefaultCameraBoomLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxZoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinZoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float ZoomRate;

	UPROPERTY(EditAnywhere, Category = "Actions")
	float SleepLength;

	UPROPERTY(EditAnywhere, Category = "Actions")
	float EatLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	class UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	class USoundCue* PigEatSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool IsSleeping() { return bIsSleeping; }

	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool Value) { bCanMove = Value; }

	UFUNCTION(BlueprintCallable)
	bool CanMove() { return bCanMove; }

	UFUNCTION(BlueprintCallable)
	bool IsShakingTree() { return bIsShakingTree; }

	UFUNCTION(BlueprintCallable)
	bool IsEating() { return bIsEating; }

	void StartSleep();

	void EndSleep();

	void StartShakingTree();

	UFUNCTION(BlueprintCallable)
	void EndShakingTree();

	void StartEating();

	UFUNCTION(BlueprintCallable)
	void EndEating();

	void Interact();

	void ZoomCamera(float Value);

	void LookAt(AActor* OtherActor);

	void ToggleHighlight(bool Value);

	FORCEINLINE void SetInteractableActor(class AInteractable* Actor) { InteractableActor = Actor; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AInteractable* GetInteractableActor() { return InteractableActor; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AAnimalCharacter* GetPossessableCharacter() { return PossessableCharacter; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetNumApplesEaten() { return NumApplesEaten; }

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool bIsSleeping;
	bool bCanMove;
	bool bIsShakingTree;
	bool bIsEating;
	FTimerHandle SleepTimerHandle;
	FTimerHandle EatTimerHandle;

	class AInteractable* InteractableActor;

	class AAnimalCharacter* PossessableCharacter;

	int32 NumApplesEaten;

	class USkeletalMeshComponent* SkeletalMesh;
};
