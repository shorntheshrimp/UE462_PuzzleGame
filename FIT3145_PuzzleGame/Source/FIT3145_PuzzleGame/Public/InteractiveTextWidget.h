// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractiveTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIT3145_PUZZLEGAME_API UInteractiveTextWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* interactiveText;
};
