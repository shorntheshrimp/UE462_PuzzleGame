// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(2.0f);
	CollisionComp->SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	//set mesh and materail
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Compnent"));
	Scene->SetupAttachment(RootComponent);
	GrenadeVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GrenadeVisual->SetupAttachment(Scene);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	if (BulletVisualAsset.Succeeded())
	{
		GrenadeVisual->SetStaticMesh(BulletVisualAsset.Object);
	}

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 9000.f;
	ProjectileMovement->MaxSpeed = 9000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0.0;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	countExplode += DeltaTime;
	if (countExplode >= explodeTime)
	{
		explodeGrenade();
	}

}

void AGrenade::explodeGrenade()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGhost::StaticClass(), FoundGhosts);
	for (int i = 0; i < FoundGhosts.Num(); i++)
	{
		FVector pawnPosition = FoundGhosts[i]->GetActorLocation() - GetActorLocation();
		float distance = pawnPosition.Size();

		AGhost* nearGhost = Cast<AGhost>(FoundGhosts[i]);

		if (distance < explodeRadius)
		{
			UE_LOG(LogTemp, Warning, TEXT("stun!"));

			nearGhost->ghostStun = true;

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeSpark, GetActorLocation(), FRotator::ZeroRotator, true);
			Destroy();
		}

	}
}



