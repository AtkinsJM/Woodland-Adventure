// Fill out your copyright notice in the Description page of Project Settings.

#include "Tree.h"
#include "Components/BoxComponent.h"

// Fill out your copyright notice in the Description page of Project Settings.

// Sets default values
ATree::ATree()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CanopyVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Canopy Volume"));
	CanopyVolume->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATree::Interact()
{
	Super::Interact();
	UE_LOG(LogTemp, Warning, TEXT("Tree interaction occurred"));
}

void ATree::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ATree::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

