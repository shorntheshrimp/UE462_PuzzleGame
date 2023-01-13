// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MyPickUpActor.generated.h"

class ABrother_Character;

UCLASS()
class FIT3145_PUZZLEGAME_API AMyPickUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPickUpActor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MyMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* HoldingComp;

	UPROPERTY(VisibleAnywhere)
	ACharacter* MyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float throwStrength;
	
	UFUNCTION()
	void Pickup();

	bool bHolding;
	bool bGravity;

	FVector ForwardVector;

};
