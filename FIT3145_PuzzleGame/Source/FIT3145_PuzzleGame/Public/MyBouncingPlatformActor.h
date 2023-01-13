// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MyBouncingPlatformActor.generated.h"

class AFIT3145_PuzzleGameCharacter;

UCLASS()
class FIT3145_PUZZLEGAME_API AMyBouncingPlatformActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBouncingPlatformActor();

	UPROPERTY(EditAnywhere, Category = "VisibleComponents")
	USceneComponent* objRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisibleComponents")
	UBoxComponent* BoxColl;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bouncingStrength;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
