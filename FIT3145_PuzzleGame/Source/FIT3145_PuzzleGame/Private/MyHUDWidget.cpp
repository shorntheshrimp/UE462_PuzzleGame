// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUDWidget.h"

UMyHUDWidget::UMyHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UMyHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//set first objective
	setObjective("Find and go through the hole in the room... where could it be?");

	tempNoticeTextLabel->SetRenderOpacity(0.f);
	
}

void UMyHUDWidget::setObjective(FString objectiveString)
{
	if (objective_text_widget)
	{
		objective_text_widget->SetText(FText::FromString(objectiveString));
	}
}

void UMyHUDWidget::setTempNotice(FString tempString)
{
	if (tempNoticeTextLabel)
	{
		tempNoticeTextLabel->SetText(FText::FromString(tempString));
	}
}
