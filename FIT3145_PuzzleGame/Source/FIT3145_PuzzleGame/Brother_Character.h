// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FIT3145_PuzzleGameCharacter.h"
#include ".//Grenade.h"

#include "Brother_Character.generated.h"

class MyPickUpActor;

/**
 * 
 */
UCLASS()
class FIT3145_PUZZLEGAME_API ABrother_Character : public AFIT3145_PuzzleGameCharacter
{
	GENERATED_BODY()
	
public:
	ABrother_Character(const class FObjectInitializer& ObjectInitializer);
	
	/** Holding Component */
	UPROPERTY(EditAnywhere)
	class USceneComponent* HoldingComponent;

	UPROPERTY(EditAnywhere)
	class AMyPickUpActor* CurrentItem;

	/** Location where projectiles should spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		USceneComponent* TP_MuzzleLocation;

	/** muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	/** Greanade class to spawn */
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class AGrenade> Grenade;

	int grenadeNumber = 999999;

	//bool bCanMove;
	bool bHoldingItem;

	FVector HoldingComp;
	//FRotator LastRotation;

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

	void throwGrenade();
};
