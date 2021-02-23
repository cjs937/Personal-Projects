// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"
#include "PilgrimageUnreal/PlayerStateMachine.h"
#include "PilgrimageUnreal/PlayerPawn.h"

void UPlayerStateBase::SetStateMachine(TScriptInterface<IStateMachineBase> owner)
{
	UStateBase::SetStateMachine(owner);

	//Set player state machine and player pawn
	OwnerStateMachine = (UPlayerStateMachine*)_owner.Get();

	if (IsValid(OwnerStateMachine))
		PlayerPawn = OwnerStateMachine->PlayerPawn;
	else
	{
		//TODO: PRINT ERROR
	}
}