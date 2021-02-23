// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateMachine.h"
#include "PlayerPawn.h"

void UPlayerStateMachine::Initialize()
{
	PlayerPawn = (APlayerPawn*)GetOwner();

	UStateMachine::Initialize();
}