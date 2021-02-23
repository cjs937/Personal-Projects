// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "PlayerStateMachine.generated.h"

class APlayerPawn;

UCLASS()
class PILGRIMAGEUNREAL_API UPlayerStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:
	APlayerPawn* PlayerPawn;

protected:
	virtual void Initialize() override;
};
