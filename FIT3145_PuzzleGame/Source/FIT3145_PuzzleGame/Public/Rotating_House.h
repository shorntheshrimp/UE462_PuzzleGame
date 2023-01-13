// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Rotating_House.generated.h"

UCLASS()
class FIT3145_PUZZLEGAME_API ARotating_House : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotating_House();
	
	UPROPERTY(EditAnywhere)
	    UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere)
		float rotateSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
