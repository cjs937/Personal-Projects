// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerIdleState.h"
#include "PilgrimageUnreal/PlayerStateMachine.h"
#include "PlayerRunState.h"

void UPlayerIdleState::Enter()
{
	UStateBase::Enter();

	PlayerPawn->ResetMovementFlags();
	PlayerPawn->ResetJumpFlags();
	PlayerPawn->SetLocomotionState(Idle);
}

void UPlayerIdleState::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (PlayerPawn->StateFlags.bMovementAllowed && PlayerPawn->MoveAxisInput != FVector2D::ZeroVector)
	{
		OwnerStateMachine->Request(UPlayerRunState::StaticClass());
	} 
}
