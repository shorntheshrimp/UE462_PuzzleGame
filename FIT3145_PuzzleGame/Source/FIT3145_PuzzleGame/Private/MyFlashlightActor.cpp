// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFlashlightActor.h"

#include "GeometryCollection/GeometryCollectionSimulationTypes.h"

// Sets default values
AMyFlashlightActor::AMyFlashlightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objMesh = CreateDefaultSubobject<UStaticMeshComponent>("objMesh");
	RootComponent = objMesh;
	objMesh->SetCollisionProfileName(FName("NoCollision"), false);
	
	spotLight = CreateDefaultSubobject<USpotLightComponent>("spotLight");
	spotLight->SetupAttachment(objMesh);
}

// Called when the game starts or when spawned
void AMyFlashlightActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyFlashlightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyFlashlightActor::AttachToPlayer(AActor* player)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Torchlight function called!")));
	AttachToActor(player, FAttachmentTransformRules::SnapToTargetIncludingScale);

	USceneComponent* playerSceneComponent = player->FindComponentByClass<USceneComponent>();
	if (playerSceneComponent != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Torchlight attached!")));
		AttachToComponent(playerSceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("headSocket"));
	}
}

