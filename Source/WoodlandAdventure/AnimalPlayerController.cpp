// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalPlayerController.h"
#include "AnimalCharacterInputComponent.h"
#include "AnimalCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#define OUT

AAnimalPlayerController::AAnimalPlayerController()
{
	BaseCameraTurnRate = 50.0f;
	BaseCameraLookUpRate = 50.0f;
	bIsControllingCameraOnly = false;
	
	InitialRotation = FRotator(-20.0f, 0.0f, 0.0f);

	bInvertYAxis = true;
	
	TurnInterpSpeed = 5.0f;

	PossessedCharacterIndex = -1;
}

void AAnimalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!AnimalCharacterInputComponent)
	{
		AnimalCharacterInputComponent = NewObject<UAnimalCharacterInputComponent>(this);
		AnimalCharacterInputComponent->RegisterComponent();
		AnimalCharacterInputComponent->bBlockInput = false;
		AnimalCharacterInputComponent->Priority = 0;
		PushInputComponent(AnimalCharacterInputComponent);
	}

	check(AnimalCharacterInputComponent);

	AnimalCharacterInputComponent->BindAxis("ForwardMovement", this, &AAnimalPlayerController::MoveForward);
	AnimalCharacterInputComponent->BindAxis(TEXT("SidewaysMovement"), this, &AAnimalPlayerController::MoveSideways);
	AnimalCharacterInputComponent->BindAxis(TEXT("CameraLookUp"), this, &AAnimalPlayerController::LookUp);
	AnimalCharacterInputComponent->BindAxis(TEXT("CameraTurn"), this, &AAnimalPlayerController::Turn);
	AnimalCharacterInputComponent->BindAxis(TEXT("CameraZoom"), this, &AAnimalPlayerController::ZoomCamera);

	AnimalCharacterInputComponent->BindAction(TEXT("Sleep"), IE_Pressed, this, &AAnimalPlayerController::Sleep);
	AnimalCharacterInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AAnimalPlayerController::Interact);

	AnimalCharacterInputComponent->BindActionWithParam(FName("NextCharacter"), EInputEvent::IE_Pressed, this, FName("SwitchCharacter"), 1);
	AnimalCharacterInputComponent->BindActionWithParam(FName("PreviousCharacter"), EInputEvent::IE_Pressed, this, FName("SwitchCharacter"), -1);

}

void AAnimalPlayerController::OnPossess(APawn * Pawn)
{
	Super::OnPossess(Pawn);
	
	AnimalCharacter = Cast<AAnimalCharacter>(Pawn);
	ControlRotation += InitialRotation;

	if (AnimalCharacter)
	{
		//AnimalCharacter->GetCharacterMovement()->AirControl = AirControl;
	}

}

void AAnimalPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	AnimalCharacter = nullptr;
}

void AAnimalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ControlRotation += InitialRotation;

	// Finds all playable (animal) characters in the world and places them in an array for swapping character.
	FindPlayableCharacters();

	AAnimalCharacter* CurrentCharacter = Cast<AAnimalCharacter>(GetPawn());
	if (CurrentCharacter)
	{
		PlayableCharacters.Find(CurrentCharacter, OUT PossessedCharacterIndex);
	}

	if (HUDOverlayAsset != nullptr)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAnimalPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement(DeltaTime);
}

void AAnimalPlayerController::HandleMovement(float DeltaTime)
{
	//Handle movement and rotation
	if (AnimalCharacter == nullptr || MovementDirection.Size() == 0.0f) { return; }
	if (!AnimalCharacter->CanMove()) { return; }

	// Start with initial rotation that turning is relative to
	FRotator DesiredRotation = AnimalCharacter->GetActorRotation();
	DesiredRotation.Yaw = ControlRotation.Yaw;

	const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(DesiredRotation);

	float DotProduct = UKismetMathLibrary::DotProduct2D(FVector2D(1, 0), MovementDirection.GetSafeNormal());
	float YawAngle = UKismetMathLibrary::DegAcos(DotProduct);
	if (MovementDirection.Y < 0.0f) { YawAngle *= -1; }
	DesiredRotation.Yaw += YawAngle;
	
	FRotator NewRotation = FMath::RInterpTo(AnimalCharacter->GetActorRotation(), DesiredRotation, DeltaTime, TurnInterpSpeed);
	AnimalCharacter->SetActorRotation(NewRotation);
	const FVector NewForwardDirection = UKismetMathLibrary::GetForwardVector(DesiredRotation);
	AnimalCharacter->AddMovementInput(NewForwardDirection, MovementDirection.Size());
	
}

void AAnimalPlayerController::MoveForward(float Value)
{
	MovementDirection.X = Value;
}

void AAnimalPlayerController::MoveSideways(float Value)
{
	MovementDirection.Y = Value;
}

void AAnimalPlayerController::Turn(float Value)
{
	if (Value == 0.0f) { return; }
	ControlRotation.Yaw += Value * BaseCameraTurnRate * GetWorld()->GetDeltaSeconds();
}

void AAnimalPlayerController::ZoomCamera(float Value)
{
	if (!AnimalCharacter) { return; }
	AnimalCharacter->ZoomCamera(Value);
}

void AAnimalPlayerController::Sleep()
{
	if (!AnimalCharacter) { return; }
	AnimalCharacter->StartSleep();
}

void AAnimalPlayerController::Interact()
{
	if (!AnimalCharacter) { return; }
	AnimalCharacter->Interact();
}

void AAnimalPlayerController::LookUp(float Value)
{
	if (Value == 0.0f) { return; }
	if (bInvertYAxis) { Value *= -1; }
	ControlRotation.Pitch += Value * BaseCameraLookUpRate * GetWorld()->GetDeltaSeconds();
	FRotator WantedRotation = ControlRotation;
	if (WantedRotation.Pitch > 180.0f) { WantedRotation.Pitch -= 360.0f; }
	WantedRotation.Pitch = FMath::Clamp(WantedRotation.Pitch, -60.0f, 0.0f);
	ControlRotation = WantedRotation;
}

void AAnimalPlayerController::FindPlayableCharacters()
{
	for (int i = 0; i < CharacterClasses.Num(); i++)
	{
		TArray<AActor*> AnimalCharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CharacterClasses[i], OUT AnimalCharacters);
		if (AnimalCharacters.Num() > 0)
		{
			AAnimalCharacter* AnimalCharacter = Cast<AAnimalCharacter>(AnimalCharacters[0]);
			if (AnimalCharacter)
			{
				PlayableCharacters.Emplace(AnimalCharacter);
			}	
		}
	}
}

void AAnimalPlayerController::SwitchCharacter(int32 Value)
{
	PossessedCharacterIndex = (PossessedCharacterIndex += Value + PlayableCharacters.Num()) % PlayableCharacters.Num();
	AAnimalCharacter* CharacterToPossess = PlayableCharacters[PossessedCharacterIndex];
	if (CharacterToPossess)
	{
		Possess(CharacterToPossess);
	}
}