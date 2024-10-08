// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

struct FGameplayTag;
class UAuraInputConfiguration;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "CPP Settings|Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "CPP Settings|Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& Value);

	
	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* CurActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "CPP Settings|Input")
	TObjectPtr<UAuraInputConfiguration> AuraInputConfiguration;
};
