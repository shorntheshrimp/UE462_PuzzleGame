// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"
#include "FIT3145_PuzzleGame/FIT3145_PuzzleGameGameMode.h"
#include "Blueprint/UserWidget.h"
#include "FIT3145_PuzzleGame/BottomTextWidget.h"
#include "MyHUDWidget.h"
#include "MyPuzzleGameCharAnimInstance.h"

// Sets default values
AGhost::AGhost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set mesh and materail
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Compnent"));
	Scene->SetupAttachment(RootComponent);

	GhostVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GhostVisual->SetupAttachment(Scene);

	EMark = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mark"));
	EMark->SetupAttachment(Scene);
	//set mark

	this->SetActorEnableCollision(ECollisionEnabled::NoCollision);
	//this->GhostVisual->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	EMark->SetVisibility(false);

	minXAxis += this->GetActorLocation().X;
	maxXAxia += this->GetActorLocation().X;
	minYAxis += this->GetActorLocation().Y;
	maxYAxia += this->GetActorLocation().Y;
	floatingAtitude += this->GetActorLocation().Z;
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//stun count 
	if (!stuned)
	{
		stunCount += DeltaTime;

	}
	if (stunCount >= stunCD)
	{
		stunCount = 0;
		stuned = true;
	}


	if (!findAllPlayerOnce)
	{
		getARandomPoint();
		//AFIT3145_PuzzleGameCharacter* player = Cast<AFIT3145_PuzzleGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		//Target = player;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFIT3145_PuzzleGameCharacter::StaticClass(), FoundPlayers);

		findAllPlayerOnce = true;
	}

	checkPlayer();
	showFindMark(DeltaTime);

	if(ghostStun)
	{
		if (ghostStunCount <= ghostStunTime)
		{
			ghostStunCount += DeltaTime;
		}
		else
		{
			ghostStun = false;
			ghostStunCount = 0;
		}
	}
	else
	{
		//found and chase player
		if (playerFound)
		{
			if (chasing)
			{
				GhostVisual->SetMaterial(0, chasing);
			}
			showMarkOrNot = true;
			chasePlayer(DeltaTime);
		}
		else
		{
			//walk around
			if (chasing)
			{
				GhostVisual->SetMaterial(0, walking);
			}
			walkAround(DeltaTime);
		}
	}


}

void AGhost::checkPlayer()
{
	//get a closest player

	float closestDistance = 99999;
	for (int i = 0; i < FoundPlayers.Num(); i++)
	{
		FVector pawnPosition = FoundPlayers[i]->GetActorLocation() - GetActorLocation();
		float distance = pawnPosition.Size();

		AFIT3145_PuzzleGameCharacter* player = Cast<AFIT3145_PuzzleGameCharacter>(FoundPlayers[i]);

		if (distance < closestDistance)
		{
			closestDistance = distance;
			Target = player;
		}

	}


	//check is player in front of ghost
	if (Target)
	{
		FVector TargetDirection = Target->GetActorLocation() - GetActorLocation();

		if (TargetDirection.Size() <= ViewRange)
		{
			TargetDirection.Normalize();
			float DotProduct = FVector::DotProduct(GetActorForwardVector(), TargetDirection);

			if (DotProduct > 0.4 && TargetDirection.Size() <= ViewRange)
			{
				playerFound = true;
			}
		}
		if (TargetDirection.Size() >= ViewRange)
		{
			playerFound = false;
		}

	}
}

void AGhost::walkAround(float tickTime)
{
	ghostChasePlayer = false;
	float distanceToDestination = (nextDestination - GetActorLocation()).Size();

	//100 is the min distance
	if (distanceToDestination > 100 && isGhostStoped == false)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("walk"));
		FVector DestinationDirection = nextDestination - GetActorLocation();
		DestinationDirection.Normalize();

		RootComponent->SetWorldLocation(GetActorLocation() + DestinationDirection * walkSpeed * tickTime);
		RootComponent->SetWorldRotation(DestinationDirection.Rotation());
	}
	else
	{
		isGhostStoped = true;
		getARandomPoint();
	}

	if (isGhostStoped == true)
	{
		stopCount += tickTime;
	}
	if (stopCount >= stopInterval)
	{
		stopCount = 0;
		isGhostStoped = false;
	}

}

void AGhost::chasePlayer(float tickTime)
{
	ghostChasePlayer = true;
	if (Target)
	{
		FVector DestinationDirection = Target->GetActorLocation() - GetActorLocation();

		float distance;
		distance = DestinationDirection.Size();
		AFIT3145_PuzzleGameCharacter* currentTarget = Cast<AFIT3145_PuzzleGameCharacter>(Target);
		//UE_LOG(LogTemp, Warning, TEXT("1!"));

		if (distance < minDistance && currentTarget && stuned)
		{
			currentTarget->playerBeenStun = true;
			
			AFIT3145_PuzzleGameGameMode* gameMode = Cast<AFIT3145_PuzzleGameGameMode>(GetWorld()->GetAuthGameMode());
			if(gameMode)
			{
				gameMode->HUDWidget->setTempNotice("You have been stunned!");
				UWidgetAnimation* tempAni = gameMode->HUDWidget->tempNoticeLabelFadeOut;
				if(tempAni)
					gameMode->HUDWidget->PlayAnimation(tempAni, 0.f, 1, EUMGSequencePlayMode::Forward, 1);
			}
			
			stuned = false;
			//UE_LOG(LogTemp, Warning, TEXT("stun player!"));
		}

		DestinationDirection.Normalize();

		RootComponent->SetWorldLocation(GetActorLocation() + DestinationDirection * chaseSpeed * tickTime);
		RootComponent->SetWorldRotation(DestinationDirection.Rotation());
	}
}

void AGhost::getARandomPoint()
{
	CallMyTrace();
	int count = 0;
	while (true)
	{
		float x;
		float y;
		x = FMath::RandRange(minXAxis, maxXAxia);
		y = FMath::RandRange(minYAxis, maxYAxia);
		nextDestination = FVector(x, y, floatingAtitude);
		CallMyTrace();

		if (nextDestinationCanReach)
		{
			nextDestinationCanReach = false;
			return;
		}

		count++;
		if (count >= 100)
		{
			return;
		}
	}

}

void AGhost::showFindMark(float tickTime)
{
	if (ghostChasePlayer)
	{
		EMark->SetVisibility(true);
		showCount = 0;
	}
	else
	{
		if (showCount < showMarkTime)
		{
			showCount += tickTime;
		}
	}
	if (showCount > showMarkTime)
	{
		showCount = 0;
		showMarkOrNot = false;
		EMark->SetVisibility(false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("tip"));
	}
}


//***************************************************************************************************
//** Trace functions - used to detect items we are looking at in the world
//***************************************************************************************************
//***************************************************************************************************

//***************************************************************************************************
//** Trace() - called by our CallMyTrace() function which sets up our parameters and passes them through
//***************************************************************************************************

bool AGhost::Trace(
	UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel,
	bool ReturnPhysMat) 
{

	// The World parameter refers to our game world (map/level) 
	// If there is no World, abort
	if (!World)
	{
		return false;
	}

	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We don't need Physics materials 
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	// Add our ActorsToIgnore
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	// When we're debugging it is really useful to see where our trace is in the world
	// We can use World->DebugDrawTraceTag to tell Unreal to draw debug lines for our trace
	// (remove these lines to remove the debug - or better create a debug switch!)
	const FName TraceTag("MyTraceTag");
	//World->DebugDrawTraceTag = TraceTag;
	TraceParams.TraceTag = TraceTag;


	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);

	// Perform our trace
	World->LineTraceSingleByChannel
	(
		HitOut,		//result
		Start,	//start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
	);

	// If we hit an actor, return true
	return (HitOut.GetActor() != NULL);
}

//***************************************************************************************************
//** CallMyTrace() - sets up our parameters and then calls our Trace() function
//***************************************************************************************************

void AGhost::CallMyTrace()
{
	// Get the location of the camera (where we are looking from) and the direction we are looking in
	const FVector Start = GetActorLocation();
	const FVector ForwardVector = GetActorForwardVector();

	// How for in front of our character do we want our trace to extend?
	// ForwardVector is a unit vector, so we multiply by the desired distance
	//const FVector End = Start + ForwardVector * 500;
	const FVector End = nextDestination;

	// Force clear the HitData which contains our results
	FHitResult HitData(ForceInit);

	// What Actors do we want our trace to Ignore?
	TArray<AActor*> ActorsToIgnore;

	//Ignore the player character - so you don't hit yourself!
	ActorsToIgnore.Add(this);

	// Call our Trace() function with the paramaters we have set up
	// If it Hits anything
	if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false))
	{
		// Process our HitData
		if (HitData.GetActor() != this && HitData.GetActor())
		{

			//UE_LOG(LogClass, Warning, TEXT("This a testing statement. %s"), *HitData.GetActor()->GetName());
			ProcessTraceHit(HitData);
			//UE_LOG(LogClass, Warning, TEXT("ghost can not reach"));
			nextDestinationCanReach = false;
		}
		else
		{
			// The trace did not return an Actor
			// An error has occurred
			// Record a message in the error log
			//UE_LOG(LogClass, Warning, TEXT("ghost can reach"));
			nextDestinationCanReach = true;
		}
	}
	else
	{
		// We did not hit an Actor
		//ClearPickupInfo();
		//UE_LOG(LogClass, Warning, TEXT("ghost can reach"));
		nextDestinationCanReach = true;
	}

}

//***************************************************************************************************
//** ProcessTraceHit() - process our Trace Hit result
//***************************************************************************************************

void AGhost::ProcessTraceHit(FHitResult& HitOut)
{
	processTraceItem(HitOut);


}

void AGhost::processTraceItem(FHitResult& HitItem)
{
	
}
