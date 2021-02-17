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
	_initialStateID(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UStateMachine::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void UStateMachine::RegisterState(UStateKey* stateID, TSubclassOf<UStateBase> stateClass)
{
	_availableStates.Add(stateID, stateClass);
}


void UStateMachine::SetInitialState(UStateKey* stateID)
{
	_initialStateID = stateID;
}

void UStateMachine::SetLogTransitions(bool bLogTransitions)
{
	_logTransitions = bLogTransitions;
}


void UStateMachine::Initialize()
{
	UStateBase* state;

	for (auto& stateInfo : _availableStates)
	{
		UStateKey* key = stateInfo.Key;

		if (key == nullptr)
		{
			UE_LOG(LogStateMachine, Error, TEXT("A null key was specified in the \"Available States\" map. Please verify that the map is populated with the expected values."));
			continue;
		}

		if (stateInfo.Value.Get() == nullptr)
		{
			UE_LOG(LogStateMachine, Error, TEXT("A null value was specified in the \"Available States\" map for the key named \"%s\"."), *key->GetFName().ToString());
			continue;
		}

		AActor* Owner = GetOwner();
		ensureMsgf(IsValid(Owner), TEXT("StateMachine unable to get owner on state %s."), *GetNameSafe(this));

		state = NewObject<UStateBase>(Owner->GetRootComponent(), stateInfo.Value, stateInfo.Key->GetFName());
		state->SetStateMachine(this);
		state->RegisterComponent();

		_idToState.Add(stateInfo.Key, state);
	}

	if (_initialStateID != nullptr)
	{
		Request(_initialStateID);
	}
	else
	{
		UE_LOG(LogStateMachine, Warning, TEXT("The initial state ID is a null value! (%s)"), *GetOwner()->GetName());
	}
}


EStateMachineResult UStateMachine::Request(UStateKey* stateID)
{
	UStateBase* nextState;

	EStateMachineResult result = TryGetState(stateID, nextState);

	_nextStateID = stateID;
	_nextStatePtr = nextState;
		
	if (result != EStateMachineResult::SUCCESS)
	{
		if (stateID != nullptr)
		{
			UE_LOG(LogStateMachine, Error, TEXT("Failed to request state named \"%s\"!"), *stateID->GetFName().ToString());
		}
		else
		{
			UE_LOG(LogStateMachine, Error, TEXT("Unable to request a null state!"));
		}

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
				*GetNameSafe(GetOwner()), *_currentStateID->GetFName().ToString(), *FDateTime::Now().ToString(), GFrameNumber);
		}
	}

	_previousStateID = _currentStateID;
	_previousStatePtr = _currentStatePtr;

	_currentStateID = stateID;
	_currentStatePtr = nextState;
	_currentStatePtr->Enter();
	if (_logTransitions)
	{
		UE_LOG(LogStateMachine, Log, TEXT("%s entering state: %s. Time: %s, FrameCount: %d"),
			*GetNameSafe(GetOwner()), *_currentStateID->GetFName().ToString(), *FDateTime::Now().ToString(), GFrameNumber);
	}

	if(!bSuppressStateChangeEvents)
		OnStateChanged.Broadcast(EStateMachineResult::SUCCESS);
	
	return EStateMachineResult::SUCCESS;
}


EStateMachineResult UStateMachine::TryGetState(UStateKey* stateID, UStateBase* &state)
{
	TWeakObjectPtr<UStateBase>* pValue = _idToState.Find(stateID);
	TWeakObjectPtr<UStateBase> result;

	if(pValue == nullptr)
		return EStateMachineResult::STATE_DOES_NOT_EXIST;

	result = (*pValue);


	if (!result.IsValid() && bWorkAroundGarbageCollectedStateInstances)
	{
		const AActor* Owner = GetOwner();
		ensureMsgf(IsValid(Owner), TEXT("TryGetState unable to get owner for state %s."), *GetNameSafe(this));

		result = NewObject<UStateBase>(Owner->GetRootComponent(), _availableStates[stateID], stateID->GetFName());
		result->SetStateMachine(this);
		result->RegisterComponent();
			
		UE_LOG(LogStateMachine, Warning, TEXT("_idToState contained trashed state for \"%s\"! Using work-around."), *stateID->GetFName().ToString());
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

UStateKey* UStateMachine::GetCurrentID() const
{
	return _currentStateID;
}

UStateBase * UStateMachine::GetPreviousState() const
{
	return _previousStatePtr;
}

UStateKey * UStateMachine::GetPreviousID() const
{
	return _previousStateID;
}

UStateBase * UStateMachine::GetNextState() const
{
	return _nextStatePtr;
}

UStateKey * UStateMachine::GetNextID() const
{
	return _nextStateID;
}

