// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAuraAttributeSet;
class UAttributeInfo;
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CPP Settings|GAS|Attributes")
	TObjectPtr<UAttributeInfo> AttributeInfo;
	
private:
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "CPP Settings|GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;
};
