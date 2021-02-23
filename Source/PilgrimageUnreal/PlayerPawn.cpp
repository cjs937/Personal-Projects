// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerStateMachine.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetSimulatePhysics(true);
	SetRootComponent(CapsuleComponent);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());

	CameraRig = CreateDefaultSubobject<USpringArmComponent>("CameraRig");
	CameraRig->SetupAttachment(GetRootComponent());
	CameraRig->TargetArmLength = 700.0f;

	CameraMask = CreateDefaultSubobject<USceneComponent>("CameraMask");
	CameraMask->SetupAttachment(CameraRig);	

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetRootComponent());

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	StateMachine = CreateDefaultSubobject<UPlayerStateMachine>("StateMachine");
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAxisInput.X = GetInputAxisValue("L_Horizontal");
	MoveAxisInput.Y = GetInputAxisValue("L_Vertical");

	CameraAxisInput.X = GetInputAxisValue("R_Horizontal");
	CameraAxisInput.Y = GetInputAxisValue("R_Vertical");

	if (StateFlags.bCameraMovementAllowed)
	{
		UpdateCameraRig();
	}

	if (StateFlags.bMovementAllowed && MoveAxisInput != FVector2D::ZeroVector)
	{
		ApplyLocomotion(MoveSpeed);
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UWorld* World = GetWorld();

	if (IsValid(World))
		EnableInput(GetWorld()->GetFirstPlayerController());

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	//
	InputComponent->BindAxis("L_Horizontal");/*, this, &APlayerPawn::Move_Y);*/
	InputComponent->BindAxis("L_Vertical");  /*, this, &APlayerPawn::Move_X);*/
	InputComponent->BindAxis("R_Horizontal");
	InputComponent->BindAxis("R_Vertical");
}

void APlayerPawn::ApplyLocomotion(float SpeedScalar)
{
	FVector2D MoveVec = MoveAxisInput.X * (FVector2D)GetActorRightVector() + MoveAxisInput.Y * (FVector2D)GetActorForwardVector();

	MoveVec = MoveVec.X * (FVector2D)Camera->GetForwardVector() + MoveVec.Y * (FVector2D)Camera->GetRightVector();

	AddMovementInput(FVector(MoveVec.X, MoveVec.Y, 0.0f), SpeedScalar);

	//Set rotation to look in direction of movement
	FRotator Rotation = SkeletalMesh->GetComponentRotation();
	Rotation.Yaw = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, FVector(MoveVec.X, MoveVec.Y, 0.0f).GetSafeNormal()).Yaw;

	SkeletalMesh->SetWorldRotation(Rotation);
}

void APlayerPawn::UpdateCameraRig()
{
	if (!IsValid(GetWorld()))
	{
		return;
	}

	float dt = GetWorld()->GetDeltaSeconds();
	//Update camera rig rotation
	FRotator NewRotation = CameraRig->GetComponentRotation();

	NewRotation.Pitch += CameraAxisInput.Y * -1.0f * CamRotateSpeed * dt;
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, CameraPitchRestraints.X, CameraPitchRestraints.Y);

	NewRotation.Yaw += CameraAxisInput.X * CamRotateSpeed * dt;

	CameraRig->SetWorldRotation(NewRotation);

	//Smooth the actual camera to match the position of the mask
	FVector Campos = FMath::VInterpTo(Camera->GetComponentLocation(), CameraMask->GetComponentLocation(), dt, CamSmoothing);
	FRotator Camrot = FMath::RInterpTo(Camera->GetComponentRotation(), UKismetMathLibrary::FindLookAtRotation(CameraMask->GetComponentLocation(), CameraRig->GetComponentLocation()), dt, CamSmoothing);

	Camera->SetWorldLocationAndRotation(Campos, Camrot);
}