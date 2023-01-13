// Fill out your copyright notice in the Description page of Project Settings.


#include "Sister_Character.h"

#include "FIT3145_PuzzleGameGameMode.h"
#include "MyHUDWidget.h"
#include "MyPickUpActor.h"
#include "Animation/WidgetAnimation.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"

ASister_Character::ASister_Character(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	this->SphereColl->InitSphereRadius(SphereColl->GetScaledSphereRadius() * 1.65);
}

void ASister_Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Gameplay
	// -- Interact w obj
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASister_Character::ToggleItemPickup);

}

void ASister_Character::BeginPlay()
{
	Super::BeginPlay();
}

/*
void ASister_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/

void ASister_Character::ToggleItemPickup()
{
	//Setup Line trace
	Start = GetFollowCamera()->GetComponentLocation();
	ForwardVector = GetFollowCamera()->GetForwardVector();
	End = (Start + (ForwardVector * 700.f));

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	AFIT3145_PuzzleGameGameMode* gameMode = Cast<AFIT3145_PuzzleGameGameMode>(GetWorld()->GetAuthGameMode());
	
	if(GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam)) 
	{
		if(Hit.GetActor()->GetClass()->IsChildOf(AMyPickUpActor::StaticClass())) 
		{				
			if(gameMode)
			{
				gameMode->HUDWidget->setTempNotice("The sister cannot pick up objects...");
				UWidgetAnimation* tempAni = gameMode->HUDWidget->tempNoticeLabelFadeOut;
				if(tempAni)
					gameMode->HUDWidget->PlayAnimation(tempAni, 0.f, 1, EUMGSequencePlayMode::Forward, 1);
			}
		}
	}

}