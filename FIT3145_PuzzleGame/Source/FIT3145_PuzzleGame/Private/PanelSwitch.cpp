// Fill out your copyright notice in the Description page of Project Settings.

#include "PanelSwitch.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
APanelSwitch::APanelSwitch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	RootComponent = BaseMesh;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APanelSwitch::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APanelSwitch::StaticClass(), FoundActors);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APanelSwitch::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &APanelSwitch::OnOverlapEnd);
}

void APanelSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor != this && OtherActor && OtherComp)
	{
		AFIT3145_PuzzleGameCharacter* player = Cast<AFIT3145_PuzzleGameCharacter>(OtherActor);
		if (OtherActor == player && isActivedForever == false)
		{
			UE_LOG(LogTemp, Warning, (TEXT("overlap begin")));
			//float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
			isActived = true;
		}

	}
}

void APanelSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this && OtherActor && OtherComp)
	{
		AFIT3145_PuzzleGameCharacter* player = Cast<AFIT3145_PuzzleGameCharacter>(OtherActor);
		if (OtherActor == player && isActivedForever == false)
		{
			UE_LOG(LogTemp, Warning, (TEXT("overlap End")));
			isActived = false;
		}

	}
}

// Called every frame
void APanelSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isActivedForever == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Open Forever"));
	}
	else
	{
		count = 0;
		if(FoundActors.IsValidIndex(0))
		{
			for (AActor* Actor : FoundActors)
			{
				APanelSwitch* Switches = Cast<APanelSwitch>(Actor);
				if (Switches != nullptr && Switches->isActived == true)
				{
					count++;
					if(Door != nullptr)
					{
						Door->OpenDoor();
						//UE_LOG(LogTemp, Warning, TEXT("count %d"), count);
						//FTimerHandle timerHandle;
						//GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]() { Door->CloseDoor(); }, 3.f, false);
					}

				
				}
				else
				{
					if(Door != nullptr)
						Door->CloseDoor();
				}
			}
		}
		if (count >= 2)
		{
			isActivedForever = true;
		}
		//UE_LOG(LogTemp, Warning, TEXT("count %d"), count);
	}


}

