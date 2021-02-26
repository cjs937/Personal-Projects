// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "PlayerPawn.h"
#include "PlayerStateMachine.h"
#include "PlayerStateBase.generated.h"

class UPlayerStateMachine;

UCLASS()
class PILGRIMAGEUNREAL_API UPlayerStateBase : public UStateBase
{
	GENERATED_BODY()
	

public:
	bool bCanJumpOutOfState = true;

	void SetStateMachine(TScriptInterface<IStateMachineBase> owner) override;

protected:

	UPROPERTY(BlueprintReadWrite)
	UPlayerStateMachine* OwnerStateMachine;

	UPROPERTY(BlueprintReadWrite)
	APlayerPawn* PlayerPawn;
};
