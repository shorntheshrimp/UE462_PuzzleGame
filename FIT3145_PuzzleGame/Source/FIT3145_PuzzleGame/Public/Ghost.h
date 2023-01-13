// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../FIT3145_PuzzleGameCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "../FIT3145_PuzzleGameCharacter.h"


#include "Ghost.generated.h"

UCLASS()
class FIT3145_PUZZLEGAME_API AGhost : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGhost();

	/** Mesh component*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* GhostVisual;

	/** mark when find player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* EMark;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
		float ViewRange = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
		float walkSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
		float chaseSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
		float minDistance = 150.0f;
	//distance that player will be catch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
		float stopInterval = 2.0f;
	float stopCount = 0;
	bool isGhostStoped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Navigation)
		float minXAxis = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Navigation)
		float maxXAxia = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Navigation)
		float minYAxis = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Navigation)
		float maxYAxia = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Navigation)
		float floatingAtitude = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Targets)
		int targetNumber = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = skin)
		UMaterial* walking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skin)
		UMaterial* chasing;

	FVector nextDestination = FVector(0, 0, 0);

	void checkPlayer();

	//if found player, it will be true
	bool playerFound;

	void walkAround(float tickTime);
	void chasePlayer(float tickTime);
	void getARandomPoint();

	//find all player in map when game start
	bool findAllPlayerOnce = false;
	//player list
	TArray<AActor*> FoundPlayers;

	UPROPERTY(EditAnywhere)
		AFIT3145_PuzzleGameCharacter* Target;

	bool showMarkOrNot = false;
	bool ghostChasePlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
		float showMarkTime = 3.0f;

	float showCount = 0;
	void showFindMark(float tickTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSetting)
		bool foundPlayer = false;

	//for ghost
	bool ghostStun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSetting)
		float ghostStunTime = 10.0f;

	float ghostStunCount = 0.0f;


	//for player
	//stun for every 10 seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSetting)
		bool stuned = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSetting)
		float stunTime = 5.0f;

	float stunCount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSetting)
		float stunCD = 10;

	float nextDestinationDistance = 0;
	bool nextDestinationCanReach = false;

	bool Trace
	(
		UWorld* World,
		TArray<AActor*>& ActorsToIgnore,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut,
		ECollisionChannel CollisionChannel = ECC_Pawn,
		bool ReturnPhysMat = false
	);
	void CallMyTrace();

	void ProcessTraceHit(FHitResult& Hitout);

	//interact the item player is facing
	void interactItem();
	void processTraceItem(FHitResult& HitItem);
	//return item that player facing

};
