///////////////////////////////////////////////////////////////////////
//
// StateMachineBase.h
//
// Brief:  
//
// Author:  William Roberts
// Created: 2018/04/05
//
// Schell Games LLC
//
///////////////////////////////////////////////////////////////////////.

#pragma once

#include "CoreMinimal.h"
#include "StateKey.h"
#include "UObject/Interface.h"
#include "StateMachineBase.generated.h"



UENUM(BlueprintType)
enum class EStateMachineResult : uint8
{
	NONE = 0,
	SUCCESS = 1,
	STATE_REFERENCE_NULL = 100,
	STATE_DOES_NOT_EXIST = 101
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UStateMachineBase : public UInterface
{
	GENERATED_BODY()
};


/**
 * Basic interface to control the State Machine.
 */
class PILGRIMAGEUNREAL_API IStateMachineBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual EStateMachineResult Request(UStateKey* stateID) = 0;

	UFUNCTION(BlueprintCallable)
	virtual UStateKey* GetCurrentID() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual UStateKey* GetPreviousID() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual UStateKey* GetNextID() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual EStateMachineResult TryGetState(UStateKey* stateID, class UStateBase* &state) = 0;
};