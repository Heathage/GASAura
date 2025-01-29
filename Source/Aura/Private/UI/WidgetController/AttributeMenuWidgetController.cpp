// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AruaAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAruaAttributeSet* AS = CastChecked<UAruaAttributeSet>(AttributeSet);

	check(AttributeInfo);

	TArray<FAuraAttributeInfo> AttributeInformation = AttributeInfo->GetAttributeInfo();

	for (FAuraAttributeInfo Attribute : AttributeInformation)
	{

		AttributeInfoDelegate.Broadcast(Attribute);
	}

	//FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	//Info.AttributeValue = AS->GetStrength();

	//AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}
