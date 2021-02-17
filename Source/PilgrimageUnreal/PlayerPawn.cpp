// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "StateMachine.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

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

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent>("MovementComponent");
	StateMachine = CreateDefaultSubobject<UStateMachine>("StateMachine");
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

	//AddMovementInput(FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.0f), MoveSpeed);
	//
	//CurrentVelocity = FVector::ZeroVector;

	//CurrentVelocity.Z = -9.8f;
	//FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	//SetActorLocation(NewLocation, true);	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	//
	//InputComponent->BindAxis("L_Horizontal", this, &APlayerPawn::Move_Y);
	//InputComponent->BindAxis("L_Vertical", this, &APlayerPawn::Move_X);
}

void APlayerPawn::Move_X(float AxisValue)
{	
	CurrentVelocity.X = AxisValue; //* MoveSpeed;
}

void APlayerPawn::Move_Y(float AxisValue)
{
	CurrentVelocity.Y = AxisValue; //* MoveSpeed;
}
