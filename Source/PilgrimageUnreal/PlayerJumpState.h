// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "PlayerJumpState.generated.h"

/**
 * 
 */
UCLASS()
class PILGRIMAGEUNREAL_API UPlayerJumpState : public UPlayerStateBase
{
	GENERATED_BODY()

	float JumpStartTime;
	//UPlayerJumpState(bool bStartFalling);

	//bool bStartFromFallState = false;

	void Enter() override;
	//void Exit() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
};
