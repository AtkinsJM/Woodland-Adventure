// Fill out your copyright notice in the Description page of Project Settings.


#include "Gate.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGate::AGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	bIsClosed = true;
	InterpSpeed = 1.0f;
}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();

	FRotator StartRotation = GetActorRotation();
	StartRotation.Yaw = bIsClosed ? ClosedYaw : OpenYaw;
	SetActorRotation(StartRotation);
}

// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleRotation(DeltaTime);
}

void AGate::HandleRotation(float DeltaTime)
{
	FRotator CurrentRotation = GetActorRotation();
	FRotator DesiredRotation = CurrentRotation;
	DesiredRotation.Yaw = bIsClosed ? ClosedYaw : OpenYaw;

	if (DesiredRotation != CurrentRotation)
	{
		SetActorRotation(FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, InterpSpeed));
	}
}

void AGate::Interact()
{
	Super::Interact();
	bIsClosed = !bIsClosed;
	InteractionPrompt = bIsClosed ? "Open" : "Close";
}

void AGate::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AGate::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

