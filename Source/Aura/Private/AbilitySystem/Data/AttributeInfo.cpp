 // Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

 FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
 {
	 for(const FAuraAttributeInfo& info : AttributeInformation)
	 {
		 if(info.AttributeTag.MatchesTagExact(AttributeTag))
		 {
			 return info;
		 }
	 }

 	 //如果没有找到并且bLogNotFound = true就打印error日志
 	 if(bLogNotFound)
 	 {
	 	 UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
 	 }

 	return FAuraAttributeInfo();
 }
