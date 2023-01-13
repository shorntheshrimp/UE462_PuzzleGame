// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIT3145_PuzzleGameCharacter.h"
#include "FIT3145_PuzzleGameGameMode.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyFlashlightActor.h"

#include "Components/PoseableMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine.h"

#include "Blueprint/UserWidget.h"
#include "BottomTextWidget.h"
#include "MyHUDWidget.h"
#include "MyPuzzleGameCharAnimInstance.h"

#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AFIT3145_PuzzleGameCharacter

AFIT3145_PuzzleGameCharacter::AFIT3145_PuzzleGameCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::CapsuleComponentName));
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//RootSceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("RootSceneComponent"));

	//does not work...
	//GetCapsuleComponent()->RemoveFromRoot();
	//GetCapsuleComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//GetCapsuleComponent()->SetupAttachment(RootComponent);
	//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.85f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	//capsuleColl = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsuleColl"));
	//capsuleColl->SetupAttachment(RootComponent);
	//capsuleColl->InitCapsuleSize(42.f, 96.0f);
	//capsuleColl->SetCollisionProfileName(TEXT("BlockAll"), true);

	//this->SetActorEnableCollision(true);
	//this->GetMesh()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//this->GetMesh()->BodyInstance.SetResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	//Possessing other char
	SphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColl"));
	SphereColl->SetupAttachment(RootComponent);
	SphereColl->InitSphereRadius(1000.f);

	//Setting Widget Blueprint if nothing is set in editor
	UObject* ClassPackage = ANY_PACKAGE;
	UClass* Result = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/ThirdPersonCPP/Blueprints/BottomTextWidget_BP.BottomTextWidget_BP_C"), nullptr, LOAD_None, nullptr);
	UClass* WidgetBPRef = FindObject<UClass>(ClassPackage, TEXT("/Game/ThirdPersonCPP/Blueprints/BottomTextWidget_BP.BottomTextWidget_BP_C"));
	if (WidgetBPRef != nullptr)
		if (WidgetClass == nullptr)
			WidgetClass = WidgetBPRef;

	//Setting flashlight Blueprint if nothing is set in editor
	UObject* ClassPackage2 = ANY_PACKAGE;
	UClass* Result2 = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/ThirdPersonCPP/Blueprints/Misc/MyFlashlightActorBP.MyFlashlightActorBP_C"), nullptr, LOAD_None, nullptr);
	UClass* FlashlightBPRef = FindObject<UClass>(ClassPackage2, TEXT("/Game/ThirdPersonCPP/Blueprints/Misc/MyFlashlightActorBP.MyFlashlightActorBP_C"));
	if (FlashlightBPRef != nullptr)
		if (flashLightActor == nullptr)
			flashLightActor = FlashlightBPRef;
	
	//so character still falls even after switching
	this->GetCharacterMovement()->bRunPhysicsWithNoController = true;

	static ConstructorHelpers::FObjectFinder<USoundCue> onSoundFX(TEXT("SoundCue'/Engine/VREditor/Sounds/UI/Click_on_Button_Cue.Click_on_Button_Cue'"));
	if (onSoundFX.Succeeded())
		toggleFlashLightOnSFX = onSoundFX.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> offSoundFX(TEXT("SoundCue'/Engine/VREditor/Sounds/VR_click2_Cue.VR_click2_Cue'"));
	if (offSoundFX.Succeeded())
		toggleFlashLightOffSFX = offSoundFX.Object;

	//find animation BP for char
	//static ConstructorHelpers::FObjectFinder<UBlueprint>animBPClass(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));
	//if (animBPClass.Succeeded())
	//	animBP = animBPClass.Object;

	//PoseMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PosebleMesh"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh>ActorMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	//PoseMesh->SetupAttachment(RootComponent);
	//PoseMesh->SetSkeletalMesh(ActorMesh.Object);
	
}

void AFIT3145_PuzzleGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* tempPC = Cast<APlayerController>(this->GetWorld()->GetFirstLocalPlayerFromController());
	if(tempPC)
	{
		UPlayerInput* playerInput = tempPC->PlayerInput;
		if(playerInput)
			playerInput->SetMouseSensitivity(playerInput->GetMouseSensitivity()-50.f);
	}
	
	SphereColl->OnComponentBeginOverlap.AddDynamic(this, &AFIT3145_PuzzleGameCharacter::OnOverlapBegin);
	SphereColl->OnComponentEndOverlap.AddDynamic(this, &AFIT3145_PuzzleGameCharacter::OnOverlapEnd);
	
	//if (this->IsPlayerControlled()) 
	//	charIsActive = true;

	if (IsValid(WidgetClass) && this->IsPlayerControlled())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Widget class found!")));
		textWidget = CreateWidget<UBottomTextWidget>(GetWorld(), WidgetClass);
	}
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFIT3145_PuzzleGameCharacter::StaticClass(), FoundActors);
	if (FoundActors.IsValidIndex(0))
	{
		for (AActor* gameActor : FoundActors)
		{
			if (gameActor != this && this->IsPlayerControlled())
			{
				float dist = FVector::Dist(this->GetActorLocation(), gameActor->GetActorLocation());
				if (dist <= 700.f)
				{
					ShowSwitchCharTip(true);
					
					AFIT3145_PuzzleGameCharacter* switchChar = Cast<AFIT3145_PuzzleGameCharacter>(gameActor);
					ActorToSwitch = switchChar;
				}
			}
		}
	}
     
}

void AFIT3145_PuzzleGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	playerCon = GetController();
	
	//if(IsPlayerControlled())
	//	isBeingControlled = true;
	//else if(!IsPlayerControlled())
	//	isBeingControlled = false;

	HeadRotateToCursor(DeltaTime);

	//count stun time

	if (stunCount >= stunTime)
	{
		stunCount = 0;
		playerBeenStun = false;
		UE_LOG(LogTemp, Warning, TEXT("release!"));
	}

	if (playerBeenStun)
	{
		stunCount += DeltaTime;

		FString TheFloatStr = FString::SanitizeFloat(stunCount);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);
	}


}

//////////////////////////////////////////////////////////////////////////
// Input

void AFIT3145_PuzzleGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Gameplay
	// -- Switch char
	PlayerInputComponent->BindAction("SwitchChar", IE_Pressed, this, &AFIT3145_PuzzleGameCharacter::SwitchChar);
	// -- Torchlight
	PlayerInputComponent->BindAction("Torchlight", IE_Pressed, this, &AFIT3145_PuzzleGameCharacter::ToggleTorchlight);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFIT3145_PuzzleGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFIT3145_PuzzleGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFIT3145_PuzzleGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFIT3145_PuzzleGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFIT3145_PuzzleGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFIT3145_PuzzleGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFIT3145_PuzzleGameCharacter::OnResetVR);
}


void AFIT3145_PuzzleGameCharacter::OnResetVR()
{
	// If FIT3145_PuzzleGame is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in FIT3145_PuzzleGame.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFIT3145_PuzzleGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AFIT3145_PuzzleGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AFIT3145_PuzzleGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFIT3145_PuzzleGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFIT3145_PuzzleGameCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFIT3145_PuzzleGameCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFIT3145_PuzzleGameCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor && OtherComp && this->GetController() != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Overlapped!")));
		
		AFIT3145_PuzzleGameCharacter* switchActor = Cast<AFIT3145_PuzzleGameCharacter>(OtherActor);
		if (switchActor)
		{
			ShowSwitchCharTip(true);
			ActorToSwitch = switchActor;
		}
	}
}

void AFIT3145_PuzzleGameCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this && OtherActor && OtherComp && this->GetController() != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Overlapped Ended...")));

		AFIT3145_PuzzleGameCharacter* switchActor = Cast<AFIT3145_PuzzleGameCharacter>(OtherActor);
		if (switchActor)
		{
			ShowSwitchCharTip(false);
			ActorToSwitch = nullptr;
		}
	}
}

void AFIT3145_PuzzleGameCharacter::SwitchChar()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Switch Char initiated.")));
	AController* controller = GetController();
		
	if (controller && ActorToSwitch != nullptr)
	{
		//ActorToSwitch->charIsActive = true;
		//this->charIsActive = false;
		ActorToSwitch->ActorToSwitch = this;
		
		if (IsValid(WidgetClass))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Widget class found!")));
			this->textWidget->RemoveFromParent();
			this->textWidget = nullptr;
			ActorToSwitch->textWidget = CreateWidget<UBottomTextWidget>(GetWorld(), WidgetClass);
			ActorToSwitch->textWidget->AddToViewport();
		}
		
		controller->UnPossess();
		controller->Possess(ActorToSwitch);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Char switched!")));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Switch Char denied... get in range")));
		AFIT3145_PuzzleGameGameMode* gameMode = Cast<AFIT3145_PuzzleGameGameMode>(GetWorld()->GetAuthGameMode());
		if(gameMode)
		{
			gameMode->HUDWidget->setTempNotice("Not in range to switch...");
			UWidgetAnimation* tempAni = gameMode->HUDWidget->tempNoticeLabelFadeOut;
			if(tempAni)
				gameMode->HUDWidget->PlayAnimation(tempAni, 0.f, 1, EUMGSequencePlayMode::Forward, 1);
		}
	}
}

//if true, show bottom widget, if not dont show/remove bottom widget from viewport
void AFIT3145_PuzzleGameCharacter::ShowSwitchCharTip(bool show)
{
	if (textWidget && this->GetController() != nullptr)
	{
		if(show)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("text widget found!")));
			textWidget->AddToViewport();
		}
		else if (!show)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("textwidget abut to b removed!")));
			textWidget->RemoveFromParent();
		}
	}

}

void AFIT3145_PuzzleGameCharacter::ToggleTorchlight()
{
	if(!torchLightOn)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), toggleFlashLightOnSFX, this->GetActorLocation());
		AMyFlashlightActor* flashLight = GetWorld()->SpawnActor<AMyFlashlightActor>(flashLightActor);
		if(flashLight)
		{
			flashLightRef = flashLight;
			flashLightRef->AttachToPlayer(this);
			torchLightOn = true;
		}
	}
	else if (torchLightOn)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), toggleFlashLightOffSFX, this->GetActorLocation());
		flashLightRef->Destroy();
		torchLightOn = false;
	}
	
}

void AFIT3145_PuzzleGameCharacter::HeadRotateToCursor(float deltaTime)
{
	//** try to rotate bone via custom pose
	//FRotator Rotation(0.0, 0.0, 45.0);
	//FName neck("neck_01");
	//PoseMesh->SetBoneRotationByName(neck, Rotation, EBoneSpaces::ComponentSpace);

	FRotator offsetRotation = FRotator(0,0,0);
	offsetRotation.Yaw = FMath::ClampAngle(this->GetControlRotation().Yaw - this->GetActorRotation().Yaw, -179.9f, -180.f);
	//offsetRotation.Yaw = this->GetControlRotation().Yaw - this->GetActorRotation().Yaw;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Neck Yaw: %f"), offsetRotation.Yaw));
	offsetRotation.Roll = this->GetControlRotation().Pitch * -1;

	UMyPuzzleGameCharAnimInstance* pAnimInst = Cast<UMyPuzzleGameCharAnimInstance>(this->GetMesh()->GetAnimInstance());
	if (pAnimInst)
	{
		pAnimInst->Skeleton_NeckRotation = offsetRotation;

		FRotator actorRot = this->GetActorRotation();
		if (offsetRotation.Yaw >= 90)
		{
			actorRot.Yaw += 90.f;
			SetActorRotation(FMath::RInterpTo(this->GetActorRotation(), actorRot, deltaTime, 1));
		}

		if (offsetRotation.Yaw <= -90)
		{
			actorRot.Yaw += -90.f;
			SetActorRotation(FMath::RInterpTo(this->GetActorRotation(), actorRot, deltaTime, 1));
		}
	}

}

//***************************************************************************************************
//** Trace functions - used to detect items we are looking at in the world
//***************************************************************************************************
//***************************************************************************************************

//***************************************************************************************************
//** Trace() - called by our CallMyTrace() function which sets up our parameters and passes them through
//***************************************************************************************************

bool AFIT3145_PuzzleGameCharacter::Trace(
	UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel = ECC_Pawn,
	bool ReturnPhysMat = false
) {

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
	World->DebugDrawTraceTag = TraceTag;
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

void AFIT3145_PuzzleGameCharacter::CallMyTrace()
{
	// Get the location of the camera (where we are looking from) and the direction we are looking in
	const FVector Start = GetFollowCamera()->GetComponentLocation();
	const FVector ForwardVector = GetFollowCamera()->GetForwardVector();

	// How for in front of our character do we want our trace to extend?
	// ForwardVector is a unit vector, so we multiply by the desired distance
	const FVector End = Start + ForwardVector * 500;

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
		if (HitData.GetActor())
		{

			UE_LOG(LogClass, Warning, TEXT("This a testing statement. %s"), *HitData.GetActor()->GetName());
			//ProcessTraceHit(HitData);

		}
		else
		{
			// The trace did not return an Actor
			// An error has occurred
			// Record a message in the error log
		}
	}
	else
	{
		// We did not hit an Actor
		//ClearPickupInfo();
	}

}
