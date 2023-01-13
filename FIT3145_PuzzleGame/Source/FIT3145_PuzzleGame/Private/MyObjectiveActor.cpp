// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObjectiveActor.h"
#include "MyHUDWidget.h"
#include "FIT3145_PuzzleGame/FIT3145_PuzzleGameGameMode.h"
#include "FIT3145_PuzzleGame/FIT3145_PuzzleGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

USoundCue* objectiveClearedSFX;

// Sets default values
AMyObjectiveActor::AMyObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxColl = CreateDefaultSubobject<UBoxComponent>("boxColl");
	boxColl->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundFX(TEXT("SoundCue'/Engine/VREditor/Sounds/VR_click3_Cue.VR_click3_Cue'"));
	objectiveClearedSFX = SoundFX.Object;
}

// Called when the game starts or when spawned
void AMyObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	boxColl->OnComponentBeginOverlap.AddDynamic(this, &AMyObjectiveActor::OnOverlapBegin);
	boxColl->OnComponentEndOverlap.AddDynamic(this, &AMyObjectiveActor::OnOverlapEnd);
}

// Called every frame
void AMyObjectiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyObjectiveActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor && OtherComp && !isCompleted)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OverlappedonObjectivesActor!")));
		
		AFIT3145_PuzzleGameCharacter* playerChar = Cast<AFIT3145_PuzzleGameCharacter>(OtherActor);
		if (playerChar && playerChar->GetController() != nullptr)
		{
			if (objectiveClearedSFX)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), objectiveClearedSFX, playerChar->GetActorLocation());
			}
			
			AFIT3145_PuzzleGameGameMode* gameMode = Cast<AFIT3145_PuzzleGameGameMode>(GetWorld() -> GetAuthGameMode());
			if(gameMode != nullptr)
			{
				UWidgetAnimation* blinkAni = gameMode->HUDWidget->blink;
				
				if (gameMode->HUDWidget)
				{
					gameMode->HUDWidget->setObjective(objectiveText);
					gameMode->HUDWidget->PlayAnimation(blinkAni, 0.f, 3, EUMGSequencePlayMode::Forward, 1);
					
					isCompleted = true;
				}
			}
		}
	}
}

void AMyObjectiveActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*
	if (OtherActor != this && OtherActor && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Overlapped Ended...")));

		AFIT3145_PuzzleGameCharacter* playerChar = Cast<AFIT3145_PuzzleGameCharacter>(OtherActor);
		if (playerChar && playerChar->GetController() != nullptr)
		{

		}
	}
	*/
}