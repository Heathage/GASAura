// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (CursorHit.bBlockingHit)
	{
		LastActor = ThisActor;

		ThisActor = CursorHit.GetActor();

	//	/**
	//	* Line trace from cursor. There are several scenarios:
	//	* A. Last Actor is null && This Actor is null
	//	*	- Do Nothing.
	//	* B. Last Actor is null && This Actor is valid.
	//	*	- Highlight This Actor.
	//	* C. Last Actor is valid && This Actor is null.
	//	*	- UnHighlight Last Actor.
	//	* D. Both Actors are valid, but Last Actor != This Actor.
	//	*	- Unhighlight Last Actor, and Highlight This Actor.
	//	* E. Both actors are valid, and are the same actor.
	//	*	- Do nothing.
	//	*/

		if (LastActor == nullptr)
		{
			if (ThisActor)
			{
				// Case B
				ThisActor->HighlightActor();
			}
			else
			{
				// Case A
				// Do Nothing
			}
		}
		else
		{
			if (ThisActor == nullptr)
			{
				// Case C
				LastActor->UnHighlightActor();
			}
			else
			{
				if (LastActor != ThisActor)
				{
					// Case D
					LastActor->UnHighlightActor();
					ThisActor->HighlightActor();
				}
				else
				{
					// Case E
					// Do nothing
				}
			}
		}
	}
}

