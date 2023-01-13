// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/GameModeBase.h"
#include "FIT3145_PuzzleGameGameMode.generated.h"

UCLASS(minimalapi)
class AFIT3145_PuzzleGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFIT3145_PuzzleGameGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		AActor* mainMenuCam;

	UPROPERTY(EditAnywhere, Category = "Class Types")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	class UMyHUDWidget* HUDWidget;

protected:
	FString mainMenuLevelName;
	FString mainMenuCamName;
};



