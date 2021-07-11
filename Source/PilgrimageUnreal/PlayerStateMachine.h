// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
//#include "PlayerAttackState.h"
#include "PlayerStateMachine.generated.h"

class APlayerPawn;
//class AAttack;

UCLASS()
class PILGRIMAGEUNREAL_API UPlayerStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:
	APlayerPawn* PlayerPawn;

	//UFUNCTION(BlueprintCallable)
	//bool BeginAttackState(AAttack* AttackClass, TSubclassOf<UPlayerAttackState> StateClassOverride);
protected:
	virtual void Initialize() override;
};
