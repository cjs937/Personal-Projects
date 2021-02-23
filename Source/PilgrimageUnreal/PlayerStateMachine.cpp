// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateMachine.h"
#include "PlayerPawn.h"

void UPlayerStateMachine::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = (APlayerPawn*)GetOwner();

	//if (!IsValid(PlayerPawn))
	//{
	//	
	//	
	//
	//	//UE_LOG(LogTemp, Error, TEXT("PlayerStateMachine %s not attached to a player pawn", GetNameSafe());
	//}
}