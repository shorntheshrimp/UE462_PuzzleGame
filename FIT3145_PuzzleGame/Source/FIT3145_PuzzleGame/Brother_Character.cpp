// Fill out your copyright notice in the Description page of Project Settings.

#include "Brother_Character.h"

#include "MyPickUpActor.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

ABrother_Character::ABrother_Character(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	FVector HCRelativeLoc = HoldingComponent->GetRelativeLocation();
	HCRelativeLoc.Z = 190.f;
	HoldingComponent->SetRelativeLocation(HCRelativeLoc);
	HoldingComponent->SetupAttachment(this->GetMesh());

	TP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation 1"));
	TP_MuzzleLocation -> SetupAttachment(RootComponent);
	TP_MuzzleLocation -> SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));


	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	CurrentItem = nullptr;
	//bCanMove = true;
	//bInspecting = false;
}

void ABrother_Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Gameplay
	// -- Interact w obj
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABrother_Character::ToggleItemPickup);

	//Press Q to thorw a grenade
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ABrother_Character::throwGrenade);

}

void ABrother_Character::BeginPlay()
{
	Super::BeginPlay();
}

/*
void ABrother_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/

void ABrother_Character::ToggleItemPickup()
{
	//Setup Line trace
	Start = GetFollowCamera()->GetComponentLocation();
	ForwardVector = GetFollowCamera()->GetForwardVector();
	End = (Start + (ForwardVector * 700.f));

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if(!bHoldingItem)
	{
		if(GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam)) 
		{
			if(Hit.GetActor()->GetClass()->IsChildOf(AMyPickUpActor::StaticClass())) 
			{				
				CurrentItem = Cast<AMyPickUpActor>(Hit.GetActor());
			}
		}
		else
		{
			CurrentItem = nullptr;
		}
	}
	
	if(CurrentItem != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Interact Initiated!")));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Interact Initiated! Object Name: %s"), *FString(CurrentItem->GetName())));
		bHoldingItem = !bHoldingItem;
		CurrentItem->Pickup();

		if(!bHoldingItem)
		{
			CurrentItem = nullptr;
		}
	}
}

void ABrother_Character::throwGrenade()
{
	if (grenadeNumber > 0)
	{
		grenadeNumber -= 1;

		if (Grenade != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				const FRotator SpawnRotation_1 = this->GetActorRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation_1 = ((TP_MuzzleLocation != nullptr) ? TP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation_1.RotateVector(MuzzleOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AGrenade>(Grenade, SpawnLocation_1, SpawnRotation_1, ActorSpawnParams);
				UE_LOG(LogTemp, Warning, TEXT("throw!"));


			}
		}
	}
}
