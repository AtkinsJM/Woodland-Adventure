// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Gate.generated.h"

UCLASS()
class WOODLANDADVENTURE_API AGate : public AInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGate();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float ClosedYaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float OpenYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bIsClosed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	float  InterpSpeed;;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleRotation(float DeltaTime);

	virtual void Interact() override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:


	
};
