// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	//如果要使用Target Data必须先初始化
	UAbilitySystemGlobals::Get().InitGlobalData();
}
