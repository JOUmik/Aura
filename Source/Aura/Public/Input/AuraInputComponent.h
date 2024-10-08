// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfiguration.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfiguration* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfiguration* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for(const FAuraInputActionMapping& ActionMapping : InputConfig->AbilityInputActionMapping)
	{
		if(ActionMapping.InputAction && ActionMapping.InputTag.IsValid())
		{
			if(PressedFunc)
			{
				BindAction(ActionMapping.InputAction, ETriggerEvent::Started, Object, PressedFunc, ActionMapping.InputTag);
			}
			if(ReleasedFunc)
			{
				BindAction(ActionMapping.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, ActionMapping.InputTag);
			}
			if(HeldFunc)
			{
				BindAction(ActionMapping.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, ActionMapping.InputTag);
			}
		}
	}
}
