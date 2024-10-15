// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interfaces/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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

	AutoRun();
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

void AAuraPlayerController::AutoRun()
{
	if(!bAutoRunning) return;
	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		//判断是否到Spline终点
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = CurActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// 如果不是鼠标左键则触发对应键位的game ability逻辑
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(UAuraAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
		{
			AbilitySystemComponent->AbilityInputTagReleased(InputTag);
		}

		return;
	}

	//如果当前点击了某个敌人则触发鼠标左键的game ability逻辑
	if(bTargeting)
	{
		if(UAuraAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
		{
			AbilitySystemComponent->AbilityInputTagReleased(InputTag);
		}
	}
	// 如果鼠标轻击的话使用自动寻路
	else
	{
		APawn* ControlledPawn = GetPawn();
		if(FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for(const FVector& PointLocation : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLocation, 8.f, 8.f, FColor::Green, false, 5.f);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}
		}

		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 如果不是鼠标左键则激活对应键位的game ability
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(UAuraAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
		{
			AbilitySystemComponent->AbilityInputTagHeld(InputTag);
		}

		return;
	}
	//如果当前点击了某个敌人则激活鼠标左键的game ability
	if(bTargeting)
	{
		if(UAuraAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
		{
			AbilitySystemComponent->AbilityInputTagHeld(InputTag);
		}
	}
	//否则鼠标左键用于寻路
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if(GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}

		if(APawn* ControlPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlPawn->GetActorLocation()).GetSafeNormal();
			ControlPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAbilitySystemComponent()
{
	if(AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
}
