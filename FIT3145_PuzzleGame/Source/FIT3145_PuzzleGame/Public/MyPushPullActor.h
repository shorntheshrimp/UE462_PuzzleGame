// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "MyPushPullActor.generated.h"

class AFIT3145_PuzzleGameCharacter;
class ABrother_Character;

UCLASS()
class FIT3145_PUZZLEGAME_API AMyPushPullActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPushPullActor();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* objMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxColl;
	UPROPERTY(EditAnywhere)
	UBoxComponent* boxColl2;

	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* physicsComp;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	float objMass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isPushing;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
