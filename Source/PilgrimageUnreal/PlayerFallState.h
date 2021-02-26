// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "PlayerFallState.generated.h"

/**
 * 
 */
UCLASS()
class PILGRIMAGEUNREAL_API UPlayerFallState : public UPlayerStateBase
{
	GENERATED_BODY()

	void Enter() override;
};
