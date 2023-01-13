// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorActor.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ADoorActor::ADoorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrame Mesh");
	RootComponent = DoorFrameMesh;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door Frame");
	DoorMesh->SetupAttachment(DoorFrameMesh);
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();


	initialYaw = DoorMesh->GetRelativeRotation().Yaw;
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//OpenDoor();
}

void ADoorActor::OpenDoor()
{
	currentYaw = FMath::Lerp(currentYaw, targetYaw, 1.f * UGameplayStatics::GetWorldDeltaSeconds(this));
	FRotator DoorRotation = DoorMesh->GetRelativeRotation();
	DoorRotation.Yaw = currentYaw;
	DoorMesh->SetRelativeRotation(DoorRotation);
}

void ADoorActor::CloseDoor()
{
	currentYaw = FMath::Lerp(currentYaw, initialYaw, 1.f * UGameplayStatics::GetWorldDeltaSeconds(this));
	FRotator DoorRotation = DoorMesh->GetRelativeRotation();
	DoorRotation.Yaw = currentYaw;
	DoorMesh->SetRelativeRotation(DoorRotation);
}

