// Fill out your copyright notice in the Description page of Project Settings.


#include "BottomTextWidget.h"

UBottomTextWidget::UBottomTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBottomTextWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (textLabel)
	{
		textLabel->SetText(FText::FromString("Press R to swap characters"));
	}
	
}

/*
void UBottomTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
*/