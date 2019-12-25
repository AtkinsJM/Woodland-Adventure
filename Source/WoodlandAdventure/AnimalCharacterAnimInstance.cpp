// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalCharacterAnimInstance.h"
#include "AnimalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimalCharacterAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn && !AnimalCharacter)
	{
		AnimalCharacter = Cast<AAnimalCharacter>(Pawn);
	}
}

void UAnimalCharacterAnimInstance::UpdateAnimationProperties()
{
	if (Pawn)
	{
		if (LastYaw == 0.0f) { LastYaw = Pawn->GetActorRotation().Yaw; }
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		ForwardSpeed = LateralSpeed.Size();
		YawDelta = Pawn->GetActorRotation().Yaw - LastYaw;
		if (YawDelta > 180.0f) { YawDelta -= 360.0f; }
		if (YawDelta < -180.0f) { YawDelta += 360.0f; }
		LastYaw = Pawn->GetActorRotation().Yaw;
		
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
	}
}
