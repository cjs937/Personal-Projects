// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRunState.h"
#include "PlayerIdleState.h"

void UPlayerRunState::Enter()
{
	UStateBase::Enter();

	PlayerPawn->ResetMovementPermissions();
	PlayerPawn->StateFlags.LocomotionState = Run;
}

void UPlayerRunState::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (PlayerPawn->MoveAxisInput == FVector2D::ZeroVector)
	{
		OwnerStateMachine->Request(UPlayerIdleState::StaticClass());
	}
}