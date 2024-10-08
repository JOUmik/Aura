// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfiguration.h"

const UInputAction* UAuraInputConfiguration::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for(auto& AbilityInputAction : AbilityInputActionMapping)
	{
		if(AbilityInputAction.InputAction && AbilityInputAction.InputTag.MatchesTagExact(InputTag))
		{
			return AbilityInputAction.InputAction;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
