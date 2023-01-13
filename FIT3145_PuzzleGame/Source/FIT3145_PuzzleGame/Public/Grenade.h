// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include ".//Ghost.h"

#include "Grenade.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class FIT3145_PUZZLEGAME_API AGrenade : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		USphereComponent* CollisionComp;


	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<AActor*> FoundGhosts;

	/** Mesh component*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* GrenadeVisual;

	//Grenade will explode after 2.0 second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		float explodeTime = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		float explodeRadius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem* explodeSpark;

	float countExplode = 0.0f;

	void explodeGrenade();
};
