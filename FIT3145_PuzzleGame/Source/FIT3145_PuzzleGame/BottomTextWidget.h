// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "BottomTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIT3145_PUZZLEGAME_API UBottomTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UBottomTextWidget(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* textLabel;
	 
};
