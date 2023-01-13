// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FIT3145_PuzzleGameCharacter.h"

#include "Sister_Character.generated.h"

/**
 * 
 */
UCLASS()
class FIT3145_PUZZLEGAME_API ASister_Character : public AFIT3145_PuzzleGameCharacter
{
	GENERATED_BODY()

public:
	ASister_Character(const class FObjectInitializer& ObjectInitializer);

	FVector Start;
	FVector ForwardVector;
	FVector End;

	FHitResult Hit;
	
	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultResponseParam;
	
protected:
	virtual void BeginPlay() override;
	
	//virtual void Tick(float DeltaTime) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void ToggleItemPickup();
};
