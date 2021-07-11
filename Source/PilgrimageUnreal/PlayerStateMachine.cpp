// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateMachine.h"
#include "PlayerPawn.h"
#include "Attack.h"

void UPlayerStateMachine::Initialize()
{
	PlayerPawn = (APlayerPawn*)GetOwner();

	UStateMachine::Initialize();
}
//
//bool UPlayerStateMachine::BeginAttackState(AAttack* AttackClass, TSubclassOf<UPlayerAttackState> StateClassOverride)
//{
//	if (!IsValid(AttackClass))
//		return false;
//
//
//}
