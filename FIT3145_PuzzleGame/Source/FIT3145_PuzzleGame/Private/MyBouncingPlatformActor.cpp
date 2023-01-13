// Fill out your copyright notice in the Description page of Project Settings.



#include "MyBouncingPlatformActor.h"

#include "FIT3145_PuzzleGame/FIT3145_PuzzleGameCharacter.h"

// Sets default values
AMyBouncingPlatformActor::AMyBouncingPlatformActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objRoot = CreateDefaultSubobject<USceneComponent>(TEXT("objRoot"));
	RootComponent = objRoot;
	
	BoxColl = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColl"));
	BoxColl->SetCollisionProfileName(TEXT("Trigger"));
	BoxColl->SetupAttachment(RootComponent);
	BoxColl->bEditableWhenInherited = true;

	bouncingStrength = 500.f;
}

// Called when the game starts or when spawned
void AMyBouncingPlatformActor::BeginPlay()
{
	Super::BeginPlay();

	BoxColl->OnComponentBeginOverlap.AddDynamic(this, &AMyBouncingPlatformActor::OnOverlapBegin);
	BoxColl->OnComponentEndOverlap.AddDynamic(this, &AMyBouncingPlatformActor::OnOverlapEnd);
}

// Called every frame
void AMyBouncingPlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBouncingPlatformActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("bouncing actor overlapped!"));
		AFIT3145_PuzzleGameCharacter* player = Cast<AFIT3145_PuzzleGameCharacter>(OtherActor);
		if (player)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("up we goooo"));

			player->LaunchCharacter(FVector(0.f,0.f,bouncingStrength), false, false);
		}

	}
}

void AMyBouncingPlatformActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*
	if (OtherActor != this && OtherActor && OtherComp)
	{
	}
	*/
}