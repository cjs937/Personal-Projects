// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UCapsuleComponent;
class UStateMachine;
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class PILGRIMAGEUNREAL_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

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
	UPawnMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStateMachine* StateMachine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	
	UPROPERTY(BlueprintReadWrite)
	FVector CurrentVelocity;

	UFUNCTION(BlueprintCallable)
	void Move_X(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void Move_Y(float AxisValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
