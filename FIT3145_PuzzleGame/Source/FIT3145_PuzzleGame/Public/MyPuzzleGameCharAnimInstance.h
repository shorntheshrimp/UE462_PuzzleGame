// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyPuzzleGameCharAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class FIT3145_PUZZLEGAME_API UMyPuzzleGameCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyPuzzleGameCharAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	/** Neck Rotation, Set in Character.cpp Tick */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=CustomVariables)
	FRotator Skeleton_NeckRotation;

protected:
	
};
