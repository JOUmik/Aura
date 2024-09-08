// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "AuraPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayer : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraPlayer();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

private:
	virtual void InitAbilityActorInfo() override;
};