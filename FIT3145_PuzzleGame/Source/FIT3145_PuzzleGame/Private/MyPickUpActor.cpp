// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPickUpActor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "../Brother_Character.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMyPickUpActor::AMyPickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	MyMesh->SetSimulatePhysics(true);
	MyMesh->SetupAttachment(RootComponent);

	bHolding = false;
	bGravity = true;

	throwStrength = 25000.0;
}

// Called when the game starts or when spawned
void AMyPickUpActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> Components;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrother_Character::StaticClass(), FoundActors);
	if (FoundActors.IsValidIndex(0))
	{
		for (AActor* gameActor : FoundActors)
		{
			if (gameActor != this)
			{
				MyCharacter = Cast<ACharacter>(gameActor);
			}
		}
	}
	
	if(MyCharacter)
	{
		MyCharacter->GetComponents(Components);

		if(Components.Num() > 0)
		{
			for (auto& Comp : Components)
			{
				if(Comp->GetName() == "HoldingComponent")
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Found HoldingComponent!")));
					HoldingComp = Cast<USceneComponent>(Comp);
				}
			}
		}
	}

}

// Called every frame
void AMyPickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if(bHolding && HoldingComp)
	//{
		//SetActorLocationAndRotation(HoldingComp->GetComponentLocation(), HoldingComp->GetComponentRotation());
	//}
	
	if(MyCharacter && bHolding)
	{
		if(!MyCharacter->IsPlayerControlled())
		{
			MyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else if(MyCharacter->IsPlayerControlled())
		{
			MyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}
}

void AMyPickUpActor::Pickup()
{
	bHolding = !bHolding;	
	bGravity = !bGravity;
	MyMesh->SetEnableGravity(bGravity);
	MyMesh->SetSimulatePhysics(bHolding ? false : true);
	MyMesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if(bHolding)
	{
		if(HoldingComp)
		{
			AttachToComponent(HoldingComp,FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true), NAME_None);
			SetActorLocationAndRotation(HoldingComp->GetComponentLocation(), HoldingComp->GetComponentRotation());
		}
	}
	else if(!bHolding) 
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		if(MyCharacter)
			ForwardVector = MyCharacter->GetActorForwardVector();
		MyMesh->AddForce(ForwardVector*throwStrength*MyMesh->GetMass());
	}

}
