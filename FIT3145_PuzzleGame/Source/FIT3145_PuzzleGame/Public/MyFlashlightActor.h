// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "MyFlashlightActor.generated.h"

UCLASS()
class FIT3145_PUZZLEGAME_API AMyFlashlightActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* objMesh;

	UPROPERTY(EditAnywhere)
	USpotLightComponent* spotLight;
	
public:	
	// Sets default values for this actor's properties
	AMyFlashlightActor();

	void AttachToPlayer(AActor* player);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
