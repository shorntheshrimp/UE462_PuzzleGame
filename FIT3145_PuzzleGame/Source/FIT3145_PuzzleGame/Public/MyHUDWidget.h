// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "MyHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIT3145_PUZZLEGAME_API UMyHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMyHUDWidget(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	UFUNCTION()
	void setObjective(FString objectiveString);
	UFUNCTION()
	void setTempNotice(FString tempString);

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* blink;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* tempNoticeLabelFadeOut;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* objective_text_widget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* tempNoticeTextLabel;

};
