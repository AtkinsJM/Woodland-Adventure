// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimalType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAnimalType : uint8
{
	EAT_Fox UMETA(DisplayName = "Fox"),
	EAT_Pig UMETA(DisplayName = "Pig"),
	EAT_Doe UMETA(DisplayName = "Doe"),
	EAT_Stag UMETA(DisplayName = "Stag")
};
