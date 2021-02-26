// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "PlayerRunState.generated.h"

/**
 * 
 */
UCLASS()
class PILGRIMAGEUNREAL_API UPlayerRunState : public UPlayerStateBase
{
	GENERATED_BODY()

	void Enter() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
};
