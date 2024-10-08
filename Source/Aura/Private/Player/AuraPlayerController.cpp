// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"
#include "Interfaces/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}



void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeDate;
	InputModeDate.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeDate.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeDate);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UAuraInputComponent* InputComp = CastChecked<UAuraInputComponent>(InputComponent))
	{
		InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

		InputComp->BindAbilityActions(AuraInputConfiguration, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, Input.Y);
		ControlledPawn->AddMovementInput(RightDirection, Input.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if(!CursorHit.bBlockingHit) return;

	LastActor = CurActor;
	CurActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several scenarios
	 *  A. LastActor is null && CurActor is null
	 *      - Do nothing
	 *  B. LastActor is null && CurActor is valid
	 *      - Highlight CurActor
	 *  C. LastActor is valid && CurActor is null
	 *      - UnHightlight LastActor
	 *  D. Both Actors are valid, but LastActor != CurActor
	 *      - UnHighlight LastActor, and Highlight CurActor
	 *  E. Both Actors are valid, and are the same actor
	 *      - Do noting
	 **/
	
	if(LastActor == nullptr)
	{
		if(CurActor == nullptr)
		{
			// Case A - both are null, do noting
		}
		else
		{
			// Case B
			CurActor->HighLightActor();
		}
	}
	else
	{
		if(CurActor == nullptr)
		{
			// Case C
			LastActor->UnHighLightActor();
		}
		else if(CurActor != nullptr && LastActor != CurActor)
		{
			// Case D
			LastActor->UnHighLightActor();
			CurActor->HighLightActor();
		}
		else
		{
			// Case E - do noting
		}
		
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
}
