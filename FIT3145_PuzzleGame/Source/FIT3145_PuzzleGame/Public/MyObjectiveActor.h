// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MyObjectiveActor.generated.h"

class FIT3145_PuzzleGameCharacter;

UCLASS()
class FIT3145_PUZZLEGAME_API AMyObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyObjectiveActor();

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxColl;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	FString objectiveText;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isCompleted;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
