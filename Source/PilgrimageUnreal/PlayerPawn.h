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
class UStateBase;

#pragma region State Flags
UENUM(BlueprintType)
enum ELocomotionState
{
	Idle = 0,
	Run,
	Jump_Rise,
	Jump_Fall,
	Land,
	Wall_Hang,
	Dash
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* LedgeGrabPosition;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge Grab")
	FVector LedgeGrabOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge Grab")
	float LedgeCheckAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge Grab")
	float LedgeCheckDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge Grab")
	float LedgeCheckHeightOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge Grab")
	FVector LedgeCheckBoxSize;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	TSubclassOf<UStateBase> DashStateClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float LongDashCooldown;

	UPROPERTY(BlueprintReadWrite)
	float CurrentDashCooldown;

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

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//FName LedgeCheckBoneName;

#pragma region Functions
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void ApplyLocomotion(float SpeedScalar);

	UFUNCTION(BlueprintCallable)
	void SetMovementAllowed(bool isAllowed) { StateFlags.bMovementAllowed = isAllowed; };

	UFUNCTION(BlueprintCallable)
	void SetCameraMovementAllowed(bool isAllowed) { StateFlags.bCameraMovementAllowed = isAllowed; };

	UFUNCTION(BlueprintCallable)
	void ResetMovementFlags();

	UFUNCTION(BlueprintCallable)
	void ResetJumpFlags();

	UFUNCTION(BlueprintCallable)
	bool CheckForLedge(UPrimitiveComponent* HitComponent, FHitResult& OutTraceResult);

	UFUNCTION(BlueprintCallable)
	void GrabLedge(FHitResult LedgeTraceHit);

	UFUNCTION(BlueprintCallable)
	bool IsGrounded(FHitResult& OutHitResult);
	bool IsGrounded();

	UFUNCTION(BlueprintCallable)
	bool SetLocomotionState(ELocomotionState NewState);

	UFUNCTION(BlueprintCallable)
	ELocomotionState GetLocomotionState() { return StateFlags.LocomotionState; };

	UFUNCTION(BlueprintCallable)
	FVector GetMoveDirection();

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
	void OnDashPressed();

	UFUNCTION(BlueprintCallable)
	void UpdateJump();
#pragma endregion
};
