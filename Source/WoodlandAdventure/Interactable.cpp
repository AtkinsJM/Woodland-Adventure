// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "AnimalCharacter.h"
#include "AnimalType.h"
#include "Components/StaticMeshComponent.h"
#include "AnimalCharacter.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	InteractionSphere->SetupAttachment(GetRootComponent());

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnEndOverlap);

	InteractionRadius = 100.0f;

	bPlaySoundOnInteraction = true;

	InteractionPrompt = "Interact";
	
	bIsInteractable = true;

	NumberOfInteractions = -1;
	
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	InteractionSphere->SetSphereRadius(InteractionRadius);
	
	GetComponents<UStaticMeshComponent>(MeshesToHighlight, true);
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractable::Interact(AAnimalCharacter* InteractingCharacter)
{
	if (NumberOfInteractions != -1)
	{
		NumberOfInteractions--;
		if (NumberOfInteractions <= 0)
		{
			bIsInteractable = false;
		}
	}	
	if (InteractionSound && bPlaySoundOnInteraction)
	{
		UGameplayStatics::PlaySound2D(this, InteractionSound);
	}
}

void AInteractable::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void AInteractable::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}

bool AInteractable::IsRequiredAnimal(EAnimalType AnimalType)
{
	return (RequiredAnimalTypes.Num() == 0 || RequiredAnimalTypes.Contains(AnimalType));
}

void AInteractable::ToggleHighlight(bool Value)
{
	if (MeshesToHighlight.Num() == 0) { return; }
	for (UStaticMeshComponent* Mesh : MeshesToHighlight)
	{
		Mesh->SetRenderCustomDepth(Value);
	}
}