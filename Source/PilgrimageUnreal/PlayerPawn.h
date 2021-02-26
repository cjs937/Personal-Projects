// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class UAnimationStateData;
class UPlayerStateMachine;

#pragma region State Flags
UENUM()
enum ELocomotionState //: uint16
{
	Idle = 0,
	Run,
	Jump_Rise,
	Jump_Fall,
	Land,
	Wall_Cling
};

USTRUCT(BlueprintType)
struct PILGRIMAGEUNREAL_API FStateFlags
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<ELocomotionState> LocomotionState = Idle;

	UPROPERTY(BlueprintReadWrite)
		bool bMovementAllowed = true;

	UPROPERTY(BlueprintReadWrite)
	bool bCameraMovementAllowed = true;
};
#pragma endregion 

UCLASS()
class PILGRIMAGEUNREAL_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraRig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* CameraMask;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerStateMachine* PlayerStateMachine;

#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MidJumpMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CamRotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CamSmoothing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CameraPitchRestraints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroundCheckRayDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpStartupTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float MidJumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float MidJumpGravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float MaxJumpHoldTime;

	UPROPERTY(BlueprintReadWrite)
	int NumJumps = 0;

	UPROPERTY(BlueprintReadWrite)
	FStateFlags StateFlags;

	UPROPERTY(BlueprintReadWrite)
	FVector CurrentVelocity;

	UPROPERTY(BlueprintReadOnly)
	FVector2D MoveAxisInput;

	UPROPERTY(BlueprintReadOnly)
	FVector2D CameraAxisInput;

	UPROPERTY(BlueprintReadOnly)
	bool bJumpInputHeld;

#pragma region Functions
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ApplyLocomotion(float SpeedScalar);

	UFUNCTION(BlueprintCallable)
	void SetMovementAllowed(bool isAllowed) { StateFlags.bMovementAllowed = isAllowed; };

	UFUNCTION(BlueprintCallable)
	void SetCameraMovementAllowed(bool isAllowed) { StateFlags.bCameraMovementAllowed = isAllowed; };

	UFUNCTION(BlueprintCallable)
	void ResetMovementFlags();

	UFUNCTION(BlueprintCallable)
	bool IsGrounded();

	UFUNCTION(BlueprintCallable)
	bool IsGrounded(FHitResult& OutHitResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateCameraRig();

	UFUNCTION(BlueprintCallable)
	void OnJumpPressed();

	UFUNCTION(BlueprintCallable)
	void OnJumpReleased();

	UFUNCTION(BlueprintCallable)
	void UpdateJump();

	UFUNCTION(BlueprintCallable)
	bool SetLocoomtionState(ELocomotionState NewState);
#pragma endregion
};
