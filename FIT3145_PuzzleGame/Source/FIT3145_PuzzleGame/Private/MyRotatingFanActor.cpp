// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRotatingFanActor.h"

// Sets default values
AMyRotatingFanActor::AMyRotatingFanActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AMyRotatingFanActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (rotateSpeed == 0)
		rotateSpeed = 15.f;
}

// Called every frame
void AMyRotatingFanActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator meshRotator(0, rotateSpeed * DeltaTime, 0);
	const FQuat meshRotationQuaternion(meshRotator);
	MeshComp->AddLocalRotation(meshRotationQuaternion, false, nullptr, ETeleportType::None);
	//AddActorLocalRotation(meshRotationQuaternion, false, nullptr, ETeleportType::None);
}

