// Fill out your copyright notice in the Description page of Project Settings.


#include "Rotating_House.h"

// Sets default values
ARotating_House::ARotating_House()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARotating_House::BeginPlay()
{
	Super::BeginPlay();

	if (rotateSpeed == 0)
		rotateSpeed = 15.f;
}

// Called every frame
void ARotating_House::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FRotator meshRotator(0, rotateSpeed * DeltaTime, 0);
	FQuat meshRotationQuaternion(meshRotator);
	AddActorLocalRotation(meshRotationQuaternion, false, nullptr, ETeleportType::None);
}

