// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FIT3145_PuzzleGame/FIT3145_PuzzleGameCharacter.h"
#include "DoorActor.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanelSwitch.generated.h"

UCLASS()
class FIT3145_PUZZLEGAME_API APanelSwitch : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APanelSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
		ADoorActor* Door = nullptr;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool isActived = false;
	bool isActivedForever = false;

	TArray<AActor*> FoundActors;
	int count = 0;
};
