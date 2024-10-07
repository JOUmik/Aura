// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	if(!AuraAttributeSet)
	{
		AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	}

	check(AttributeInfo);
	
	for(auto& Pair :AuraAttributeSet->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		FGameplayAttribute Attr = Pair.Value();
		Info.AttributeValue = Attr.GetNumericValue(AuraAttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if(!AuraAttributeSet)
	{
		AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	}
	
	for(auto& Pair :AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
			Info.AttributeValue = Data.NewValue;
			AttributeInfoDelegate.Broadcast(Info);
		});
	}
	
}
