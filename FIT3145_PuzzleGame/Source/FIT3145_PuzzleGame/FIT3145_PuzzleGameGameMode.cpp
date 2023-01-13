// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIT3145_PuzzleGameGameMode.h"
#include "FIT3145_PuzzleGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"
#include "MyHUDWidget.h"

AFIT3145_PuzzleGameGameMode::AFIT3145_PuzzleGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	mainMenuLevelName = "MainMenu";
	mainMenuCamName = "MainMenuCameraActor";

	//Setting Widget Blueprint if nothing is set in editor
	UObject* ClassPackage = ANY_PACKAGE;
	UClass* Result = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/ThirdPersonCPP/Blueprints/UI/Game_HUD_BP.Game_HUD_BP_C"), nullptr, LOAD_None, nullptr);
	UClass* WidgetBPRef = FindObject<UClass>(ClassPackage, TEXT("/Game/ThirdPersonCPP/Blueprints/UI/Game_HUD_BP.Game_HUD_BP_C"));
	if (WidgetBPRef != nullptr)
		if (WidgetClass == nullptr)
			WidgetClass = WidgetBPRef;
}

void AFIT3145_PuzzleGameGameMode::BeginPlay()
{
	/*
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), foundActors);

	for (AActor* camActor : foundActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("camera: %s"), *FString(camActor->GetName())));
		if(camActor->GetName() == mainMenuCamName)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("camera found!!")));
			mainMenuCam = camActor;
		}
	}
	*/
	
	APlayerController* playerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (!MapName.IsEmpty())
	{
		/*
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("level: %s"), *FString(MapName)));
		if(MapName == mainMenuLevelName && playerCon != nullptr)
		{
			if(mainMenuCam != nullptr)
				playerCon-> SetViewTarget(mainMenuCam);
			playerCon-> UnPossess();
		}
		*/

		if(MapName != mainMenuLevelName)
		{
			if (IsValid(WidgetClass))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HUD Widget class found!")));
				this->HUDWidget = CreateWidget<UMyHUDWidget>(GetWorld(), WidgetClass);
				this->HUDWidget->AddToViewport();
			}
		}
	}
}
