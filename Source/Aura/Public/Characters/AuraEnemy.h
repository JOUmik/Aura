// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interfaces/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	
	virtual void HighLightActor() override; //<EnemyInterface>
	virtual void UnHighLightActor() override; //<EnemyInterface>

	virtual int32 GetPlayerLevel() override; //<CombatInterface>

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP Settings|Character Class Defaults", meta = (AllowPrivateAccess  =  "true"))
	int32 Level = 1;
};
