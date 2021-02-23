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
	Jump_Idle,
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
	UPlayerStateMachine* StateMachine;

#pragma endregion

	UPROPERTY(BlueprintReadWrite)
	FStateFlags StateFlags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CamRotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CamSmoothing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CameraPitchRestraints;

	UPROPERTY(BlueprintReadWrite)
	FVector CurrentVelocity;
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

	UPROPERTY(BlueprintReadOnly)
	FVector2D MoveAxisInput;

	UPROPERTY(BlueprintReadOnly)
	FVector2D CameraAxisInput;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateCameraRig();

#pragma endregion
};
