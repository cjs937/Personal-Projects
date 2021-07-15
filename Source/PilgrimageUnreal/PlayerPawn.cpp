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
#include "WorldCollision.h"
#include "PlayerStates.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

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
	
	LedgeGrabPosition = CreateDefaultSubobject<USceneComponent>("LedgeGrabPosition");
	LedgeGrabPosition->SetupAttachment(SkeletalMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetRootComponent());

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	PlayerStateMachine = CreateDefaultSubobject<UPlayerStateMachine>("StateMachine");
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentHit.AddDynamic(this, &APlayerPawn::OnComponentHit);
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

	if (StateFlags.LocomotionState == Jump_Rise || StateFlags.LocomotionState == Jump_Fall)
	{
		UpdateJump();
	}
	else if (StateFlags.LocomotionState != Wall_Hang && !IsGrounded()) //if player is not in the jump or fall state and leaves the ground
	{
		PlayerStateMachine->Request(UPlayerFallState::StaticClass());
	}

	if (StateFlags.bMovementAllowed && MoveAxisInput != FVector2D::ZeroVector)
	{
		ApplyLocomotion(IsGrounded() ? MoveSpeed : MidJumpMoveSpeed);
	}

	if (CurrentDashCooldown > 0.0f)
	{
		CurrentDashCooldown -= DeltaTime;
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UWorld* World = GetWorld();

	if (IsValid(World))
		EnableInput(GetWorld()->GetFirstPlayerController());

	InputComponent->BindAxis("L_Horizontal");
	InputComponent->BindAxis("L_Vertical");  
	InputComponent->BindAxis("R_Horizontal");
	InputComponent->BindAxis("R_Vertical");


	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerPawn::OnJumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerPawn::OnJumpReleased);
	InputComponent->BindAction("Dash", IE_Pressed, this, &APlayerPawn::OnDashPressed);
}

FVector APlayerPawn::GetMoveDirection()
{
	FVector2D MoveVec = MoveAxisInput.X * (FVector2D)GetActorRightVector() + MoveAxisInput.Y * (FVector2D)GetActorForwardVector();

	MoveVec = MoveVec.X * (FVector2D)Camera->GetForwardVector() + MoveVec.Y * (FVector2D)Camera->GetRightVector();

	return FVector(MoveVec.X, MoveVec.Y, 0.0f);
}

void APlayerPawn::ApplyLocomotion(float SpeedScalar)
{
	AddMovementInput(GetMoveDirection(), SpeedScalar);

	//Set rotation to look in direction of movement
	FRotator Rotation = SkeletalMesh->GetComponentRotation();
	Rotation.Yaw = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, GetMoveDirection()).Yaw;

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

void APlayerPawn::ResetMovementFlags()
{
	StateFlags.bMovementAllowed = true;
	StateFlags.bCameraMovementAllowed = true;
	CapsuleComponent->SetSimulatePhysics(true);
}

void APlayerPawn::ResetJumpFlags()
{
	NumJumps = 0;
}

bool APlayerPawn::IsGrounded()
{
	FHitResult HitResult;

	return IsGrounded(HitResult);
}

bool APlayerPawn::IsGrounded(FHitResult& OutHitResult)
{
	UWorld* World = GetWorld();

	if (!IsValid(World))
		return true;

	FVector TraceStart = CapsuleComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + (GetActorUpVector() * -1.0f) * GroundCheckRayDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//ActorLineTraceSingle(OutHitResult, TraceStart, TraceEnd, ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam);
	
	//DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, true);

	World->LineTraceSingleByProfile(OutHitResult, TraceStart, TraceEnd, "BlockAll", Params);

	return OutHitResult.bBlockingHit;
}

void APlayerPawn::UpdateJump()
{
	FVector Force;

	if (StateFlags.LocomotionState == Jump_Rise)
	{
		Force = FVector::UpVector * MidJumpForce;
	}
	else
	{
		Force = FVector::UpVector * -1.0f * MidJumpGravity;
	}

	CapsuleComponent->AddForce(Force);
}

void APlayerPawn::OnJumpPressed()
{
	if (NumJumps < 2)
	{
		PlayerStateMachine->Request(UPlayerJumpState::StaticClass());
	}

	bJumpInputHeld = true;
}

void APlayerPawn::OnDashPressed()
{
	if (CurrentDashCooldown <= 0.0f && IsValid(DashStateClass))
	{
		PlayerStateMachine->Request(DashStateClass);
	}

	CurrentDashCooldown = DashCooldown;
}


void APlayerPawn::OnJumpReleased()
{
	bJumpInputHeld = false;
}	

bool APlayerPawn::SetLocomotionState(ELocomotionState NewState)
{
	StateFlags.LocomotionState = NewState;

	return true;
}

bool APlayerPawn::CheckForLedge(UPrimitiveComponent* HitComponent, FHitResult& OutTraceResult)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
		return false;

	FVector PlayerGrabPos = LedgeGrabPosition->GetComponentLocation();//SkeletalMesh->GetBoneLocation(LedgeCheckBoneName);
	FVector WallPos = HitComponent->GetComponentLocation();

	float AngleToWall = FVector::DotProduct((WallPos - PlayerGrabPos).GetSafeNormal2D(), SkeletalMesh->GetForwardVector());
	AngleToWall = FMath::Acos(AngleToWall);

	if (AngleToWall > LedgeCheckAngle)
		return false;

	float WallBoundsZ = HitComponent->GetComponentLocation().Z + HitComponent->Bounds.BoxExtent.Z; //* 2;
	FVector TraceStart = PlayerGrabPos;
	TraceStart.Z = WallBoundsZ;
	TraceStart += FVector::OneVector * LedgeCheckHeightOffset;

	ETraceTypeQuery CollisionChannel = UEngineTypes::ConvertToTraceType(ECC_PhysicsBody);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::BoxTraceSingle(World, TraceStart, PlayerGrabPos, LedgeCheckBoxSize, FRotator::ZeroRotator, CollisionChannel, true,
		ActorsToIgnore, EDrawDebugTrace::None, OutTraceResult, true);

	if (OutTraceResult.bBlockingHit) //&& OutTraceResult.Location.Z >= PlayerGrabPos.Z)
	{
		return FVector::Distance(OutTraceResult.Location, LedgeGrabPosition->GetComponentLocation()) < LedgeCheckDistance;
	}
	else
	{
		return false;
	}
}

void APlayerPawn::GrabLedge(FHitResult LedgeTraceHit)
{
	EStateMachineResult RequestResult = PlayerStateMachine->Request(UPlayerWallHangState::StaticClass());
	UPlayerWallHangState* HangState = (UPlayerWallHangState*)PlayerStateMachine->GetCurrentState();

	if (RequestResult == EStateMachineResult::SUCCESS && IsValid(HangState))
	{
		FVector GrabPos = LedgeTraceHit.Location + LedgeGrabOffset;
		FVector MeshPos = SkeletalMesh->GetComponentLocation();
		FRotator GrabRot = SkeletalMesh->GetComponentRotation();
		FRotator OldRot = GrabRot;
		GrabRot.Yaw = UKismetMathLibrary::FindLookAtRotation(MeshPos, LedgeTraceHit.GetActor()->GetActorLocation()).Yaw;
		//GrabRot.Yaw = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, (LedgeTraceHit.Location - SkeletalMesh->GetComponentLocation()).GetSafeNormal()).Yaw; // .Yaw;
		//DrawDebugSphere(GetWorld(), LedgeTraceHit.GetActor()->GetActorLocation(), 10.0f, 15, FColor::Blue, true);
		//DrawDebugSphere(GetWorld(), MeshPos, 10.0f, 15, FColor::Blue, true);
		//DrawDebugLine(GetWorld(), LedgeTraceHit.GetActor()->GetActorLocation(), MeshPos, FColor::Red, true, -1.0f, (uint8)'\000', 7.0f);
		//DrawDebugSphere(GetWorld(), LedgeTraceHit.Location, 20.0f, 15, FColor::Green, true);

		SetActorLocation(GrabPos);
		//SkeletalMesh->SetWorldRotation(GrabRot);
		HangState->InitWallSnap(LedgeTraceHit);

		//UE_LOG(LogTemp, Log, TEXT("Previous: %s, Current: %s"), *OldRot.ToString(), *GrabRot.ToString());
		//SetActorLocationAndRotation(GrabPos, GrabRot);
	}
	//UE_LOG(LogTemp, Log, TEXT("Grabbed %s"), *GetNameSafe(LedgeTraceHit.GetActor()));

	
}

void APlayerPawn::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FHitResult LedgeHit;

	if (CheckForLedge(Hit.GetComponent(), LedgeHit) && StateFlags.LocomotionState == Jump_Fall)
	{
		GrabLedge(LedgeHit);
	}
}

float APlayerPawn::GetStickAngle(float XInput, float YInput)
{
	return 2 * FMath::Atan(YInput / (XInput + FMath::Sqrt((XInput * XInput) + (YInput * YInput))));
}
