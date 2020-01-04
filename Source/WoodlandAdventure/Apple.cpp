// Fill out your copyright notice in the Description page of Project Settings.


#include "Apple.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "AnimalCharacter.h"

AApple::AApple()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	StaticMesh->OnComponentHit.AddDynamic(this, &AApple::OnHit);
}


// Called when the game starts or when spawned
void AApple::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AApple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// TODO: sort this mess out!
	Root->SetWorldTransform(StaticMesh->GetComponentTransform());
}

void AApple::Interact(AAnimalCharacter* InteractingCharacter)
{
	Super::Interact(InteractingCharacter);
	Destroy();
}

void AApple::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}

void AApple::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AApple::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (DropSound)
	{
		UGameplayStatics::PlaySound2D(this, DropSound);
	}
}
