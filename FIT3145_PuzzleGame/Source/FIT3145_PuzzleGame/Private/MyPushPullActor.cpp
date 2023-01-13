// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPushPullActor.h"
#include "FIT3145_PuzzleGame/FIT3145_PuzzleGameCharacter.h"
#include "FIT3145_PuzzleGame/Brother_Character.h"

// Sets default values
AMyPushPullActor::AMyPushPullActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objMesh = CreateDefaultSubobject<UStaticMeshComponent>("objMesh");
	objMesh->SetupAttachment(RootComponent);

	boxColl = CreateDefaultSubobject<UBoxComponent>("boxColl");
	boxColl->SetupAttachment(objMesh);
	boxColl2 = CreateDefaultSubobject<UBoxComponent>("boxColl2");
	boxColl2->SetupAttachment(objMesh);
	
	physicsComp = CreateDefaultSubobject<UPhysicsConstraintComponent>("physicsComp");
	physicsComp->SetupAttachment(objMesh);
	physicsComp->ComponentName1.ComponentName = objMesh->GetFName();

	//setup physics for pushing
	//for physicsComp
	physicsComp->SetLinearXLimit(ELinearConstraintMotion::LCM_Free, 0.f);
	physicsComp->SetLinearYLimit(ELinearConstraintMotion::LCM_Free, 0.f);
	physicsComp->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
	physicsComp->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 15.f);
	physicsComp->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 15.f);
	physicsComp->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 15.f);

	if (objMass <= 0)
		objMass = 2000.f;
}

// Called when the game starts or when spawned
void AMyPushPullActor::BeginPlay()
{
	Super::BeginPlay();

	boxColl->OnComponentBeginOverlap.AddDynamic(this, &AMyPushPullActor::OnOverlapBegin);
	boxColl->OnComponentEndOverlap.AddDynamic(this, &AMyPushPullActor::OnOverlapEnd);
	boxColl2->OnComponentBeginOverlap.AddDynamic(this, &AMyPushPullActor::OnOverlapBegin);
	boxColl2->OnComponentEndOverlap.AddDynamic(this, &AMyPushPullActor::OnOverlapEnd);

}

// Called every frame
void AMyPushPullActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPushPullActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OverlappedonObjectivesActor!")));
		
		ABrother_Character* broChar = Cast<ABrother_Character>(OtherActor);
		if (broChar && broChar->IsPlayerControlled())
		{
			objMesh->SetSimulatePhysics(true);
			objMesh->SetMassOverrideInKg(NAME_None, objMass, true);
			objMesh->SetLinearDamping(5.5f);
			isPushing = true;
			broChar->isPushing = isPushing;
		}
	}
}

void AMyPushPullActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this && OtherActor && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Overlapped Ended...")));

		AFIT3145_PuzzleGameCharacter* playerChar = Cast<AFIT3145_PuzzleGameCharacter>(OtherActor);
		if (playerChar && playerChar->IsPlayerControlled())
		{
			objMesh->SetSimulatePhysics(false);
			isPushing = false;
			playerChar->isPushing = isPushing;
		}
	}
}