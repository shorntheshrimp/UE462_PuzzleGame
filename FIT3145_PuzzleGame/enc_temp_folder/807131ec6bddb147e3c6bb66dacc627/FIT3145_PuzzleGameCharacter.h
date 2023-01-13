// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "MyFlashlightActor.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"

#include "FIT3145_PuzzleGameCharacter.generated.h"

class USphereComponent;
class AFIT3145_PuzzleGameGameMode;

UCLASS(config=Game)
class AFIT3145_PuzzleGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	//AFIT3145_PuzzleGameCharacter();
	AFIT3145_PuzzleGameCharacter(const class FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Char capsule component (created)
	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent;

	//Char capsule component (created)
	//UPROPERTY(EditAnywhere)
	//UCapsuleComponent* capsuleColl;

	//Char swapping via sphere range
	UPROPERTY(EditAnywhere)
		USphereComponent* SphereColl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AController* playerCon;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//empty var to take in during collision hecks (do not change in editor)
	UPROPERTY(VisibleAnywhere, Category = "SwitchChar")
		AFIT3145_PuzzleGameCharacter* ActorToSwitch;

	UPROPERTY(EditAnywhere, Category = "Class Types")
		TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UBottomTextWidget* textWidget;

	UPROPERTY(BlueprintReadOnly)
		bool isPushing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isBeingControlled;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMyFlashlightActor> flashLightActor;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//Switch char start
	void SwitchChar();

	bool charIsActive;
	//Switch char end

	void ShowSwitchCharTip(bool show);

	void ToggleTorchlight();

	bool torchLightOn;

	AMyFlashlightActor* flashLightRef;

	USoundCue* toggleFlashLightOnSFX;
	USoundCue* toggleFlashLightOffSFX;

	void HeadRotateToCursor(float deltaTime);

	UBlueprint* animBP;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* PoseMesh;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSetting)
		bool playerBeenStun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSetting)
		float stunTime = 5.0f;

	float stunCount = 0.0f;

	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
};

