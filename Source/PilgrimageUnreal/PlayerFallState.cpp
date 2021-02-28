// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFallState.h"
#include "PlayerStates.h"

void UPlayerFallState::Enter()
{
	UStateBase::Enter();

	PlayerPawn->SetLocomotionState(Jump_Fall);
}

void UPlayerFallState::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	//If player is grounded
	if (PlayerPawn->IsGrounded())
	{
		OwnerStateMachine->Request(UPlayerIdleState::StaticClass());
	}
}
