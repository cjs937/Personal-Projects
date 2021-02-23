///////////////////////////////////////////////////////////////////////
//
// StateBase.h
//
// Brief:  
//
// Author:  William Roberts
// Created: 2018/03/26
//
// Schell Games LLC
//
///////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineBase.h"
#include "StateBase.generated.h"

class StateMachine;

/**
( * Base class for all states.
 */
UCLASS(Abstract, Blueprintable)
class PILGRIMAGEUNREAL_API UStateBase : public UActorComponent
{
	GENERATED_BODY()
protected:

	TWeakObjectPtr<UObject> _owner;

	UFUNCTION(BlueprintImplementableEvent)
	void EnterState();

	UFUNCTION(BlueprintImplementableEvent)
	void ExitState();

public:
	
	UStateBase();

	virtual void Enter();

	virtual void Exit();
	
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IStateMachineBase> GetStateMachine();
	
	virtual void SetStateMachine(TScriptInterface<IStateMachineBase> owner);

	// Variable that determines if the component tick is enabled on entry to the state.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableTickOnEntry = true;
};
