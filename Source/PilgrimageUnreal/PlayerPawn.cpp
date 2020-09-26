// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
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

	CurrentVelocity.Z = -9.8f;
	FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	SetActorLocation(NewLocation, true);	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	InputComponent->BindAxis("L_Horizontal", this, &APlayerPawn::Move_Y);
	InputComponent->BindAxis("L_Vertical", this, &APlayerPawn::Move_X);
}

void APlayerPawn::Move_X(float AxisValue)
{
	CurrentVelocity.X = AxisValue * MoveSpeed;
}

void APlayerPawn::Move_Y(float AxisValue)
{
	CurrentVelocity.Y = AxisValue * MoveSpeed;
}
