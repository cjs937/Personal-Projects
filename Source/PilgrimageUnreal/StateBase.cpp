// Fill out your copyright notice in the Description page of Project Settings.

#include  "StateBase.h"


UStateBase::UStateBase()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UStateBase::Enter() 
{
	if(bEnableTickOnEntry)
	{
		PrimaryComponentTick.SetTickFunctionEnable(true);	
	}
	EnterState();
}

void UStateBase::Exit()
{
	PrimaryComponentTick.SetTickFunctionEnable(false);
	ExitState();
}

void UStateBase::SetStateMachine(TScriptInterface<IStateMachineBase> owner)
{
	_owner = owner.GetObject();
}

TScriptInterface<IStateMachineBase> UStateBase::GetStateMachine()
{
	if (!_owner.IsValid())
		return nullptr;

	return _owner.Get();
}
