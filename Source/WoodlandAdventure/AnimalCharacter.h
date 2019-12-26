// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnimalCharacter.generated.h"

UCLASS()
class WOODLANDADVENTURE_API AAnimalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimalCharacter();

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

	void StartSleep();

	void EndSleep();

	void Interact();

	void ZoomCamera(float Value);

	FORCEINLINE void SetInteractingActor(class AInteractable* Actor) { InteractingActor = Actor; }

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool bIsSleeping;
	bool bCanMove;
	FTimerHandle SleepTimerHandle;

	class AInteractable* InteractingActor;
};
