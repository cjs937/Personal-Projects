///////////////////////////////////////////////////////////////////////
//
// StateMachine.h
//
// Brief:  Basic State Machine
//
// Author:  William Roberts
// Created: 2018/03/26
//
// Schell Games LLC
//
///////////////////////////////////////////////////////////////////////

#include "StateMachine.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogStateMachine);


UStateMachine::UStateMachine() :
	InitialState(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UStateMachine::BeginPlay()
{
	Super::BeginPlay();

	this->Initialize();
}

void UStateMachine::RegisterState(TSubclassOf<UStateBase> stateClass)
{
	AvailableStates.Add(stateClass);
}


void UStateMachine::SetInitialState(TSubclassOf<UStateBase> stateClass)
{
	InitialState = stateClass;
}

void UStateMachine::SetLogTransitions(bool bLogTransitions)
{
	_logTransitions = bLogTransitions;
}


void UStateMachine::Initialize()
{
	UStateBase* state;
	TSubclassOf<UStateBase> StateClass;

	for (int i = 0; i < AvailableStates.Num(); ++i)
	{
		StateClass = AvailableStates[i];

		if (IsValid(StateClass) && !StateMap.Contains(StateClass))
		{
			AActor* Owner = GetOwner();
			ensureMsgf(IsValid(Owner), TEXT("StateMachine unable to get owner on state %s."), *GetNameSafe(this));

			state = NewObject<UStateBase>(Owner->GetRootComponent(), StateClass, StateClass->GetFName());
			state->SetStateMachine(this);
			state->RegisterComponent();

			StateMap.Add(StateClass, state);
		}
		else
		{
			if (!IsValid(StateClass))
			{
				UE_LOG(LogStateMachine, Error, TEXT("A null key was specified in the \"Available States\" map. Please verify that the map is populated with the expected values."));
				continue;
			}
			else
			{
				UE_LOG(LogStateMachine, Error, TEXT("A duplicate key was specified in the \"Available States\" map. Please verify that the map is populated with the expected values."));
				continue;
			}
			
		}
	}

	if (IsValid(InitialState))
	{
		Request(InitialState);
	}
	else
	{
		UE_LOG(LogStateMachine, Warning, TEXT("The initial state is a null value! (%s)"), *GetOwner()->GetName());
	}
}


EStateMachineResult UStateMachine::Request(TSubclassOf<UStateBase> StateClass)
{
	UStateBase* nextState;

	EStateMachineResult result = TryGetState(StateClass, nextState);
		
	if (result != EStateMachineResult::SUCCESS)
	{
		//if (stateID != nullptr)
		//{
		//	UE_LOG(LogStateMachine, Error, TEXT("Failed to request state named \"%s\"!"), *stateID->GetFName().ToString());
		//}
		//else
		//{
		//	UE_LOG(LogStateMachine, Error, TEXT("Unable to request a null state!"));
		//}

		UE_LOG(LogStateMachine, Error, TEXT("Unable to request a null state!"));

		if(!bSuppressStateChangeEvents)
			OnStateChanged.Broadcast(result);
		
		return result;
	}

	if (_currentStatePtr != nullptr)
	{
		_currentStatePtr->Exit();
		if (_logTransitions)
		{
			UE_LOG(LogStateMachine, Log, TEXT("%s exiting state: %s. Time: %s, FrameCount: %d"), 
				*GetNameSafe(GetOwner()), *_currentStatePtr->GetFName().ToString(), *FDateTime::Now().ToString(), GFrameNumber);
		}
	}

	_previousStatePtr = _currentStatePtr;

	_currentStatePtr = nextState;
	_currentStatePtr->Enter();

	if (_logTransitions)
	{
		UE_LOG(LogStateMachine, Log, TEXT("%s entering state: %s. Time: %s, FrameCount: %d"),
			*GetNameSafe(GetOwner()), *_currentStatePtr->GetFName().ToString(), *FDateTime::Now().ToString(), GFrameNumber);
	}

	if(!bSuppressStateChangeEvents)
		OnStateChanged.Broadcast(EStateMachineResult::SUCCESS);
	
	return EStateMachineResult::SUCCESS;
}

EStateMachineResult UStateMachine::TryGetState(TSubclassOf<UStateBase> StateClass, UStateBase* &state)
{
	TWeakObjectPtr<UStateBase>* pValue = StateMap.Find(StateClass);//_idToState.Find(stateID);
	TWeakObjectPtr<UStateBase> result;

	if(pValue == nullptr)
		return EStateMachineResult::STATE_DOES_NOT_EXIST;

	result = (*pValue);


	if (!result.IsValid() && bWorkAroundGarbageCollectedStateInstances)
	{
		const AActor* Owner = GetOwner();
		ensureMsgf(IsValid(Owner), TEXT("TryGetState unable to get owner for state %s."), *GetNameSafe(this));

		result = NewObject<UStateBase>(Owner->GetRootComponent(), StateClass, StateClass->GetFName());
		result->SetStateMachine(this);
		result->RegisterComponent();
			
		UE_LOG(LogStateMachine, Warning, TEXT("_idToState contained trashed state for \"%s\"! Using work-around."), *StateClass->GetFName().ToString());
	}

	if (!result.IsValid())
		return EStateMachineResult::STATE_REFERENCE_NULL;

	state = result.Get();

	return EStateMachineResult::SUCCESS;
}

UStateBase* UStateMachine::GetCurrentState() const
{
	return _currentStatePtr;
}

UStateBase * UStateMachine::GetPreviousState() const
{
	return _previousStatePtr;
}
