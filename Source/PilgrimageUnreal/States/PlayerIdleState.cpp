// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerIdleState.h"
#include "PilgrimageUnreal/PlayerStateMachine.h"
#include "PilgrimageUnreal/PlayerPawn.h"

void UPlayerIdleState::Enter()
{
	PlayerPawn->SetMovementAllowed(true);
	PlayerPawn->SetCameraMovementAllowed(true);

	PlayerPawn->StateFlags.LocomotionState = ELocomotionState::Idle;
}

void UPlayerIdleState::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (PlayerPawn->MoveAxisInput == FVector2D::ZeroVector)
	{
		//OwnerStateMachine->
	}
}
