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

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineBase.h"
#include "StateBase.h"
#include "StateKey.h"

#include "StateMachine.generated.h"

DECLARE_EVENT_OneParam(UStateMachine, FStateChangedEvent, EStateMachineResult);

DECLARE_LOG_CATEGORY_EXTERN(LogStateMachine, Log, All);

/**
 * Basic State Machine
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class PILGRIMAGEUNREAL_API UStateMachine : public UActorComponent, public IStateMachineBase
{
	GENERATED_BODY()

	
protected:
	// The id of the active state in the state machine.
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	UStateKey* _currentStateID;
	
	// The object for the active state in the state machine.
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	UStateBase* _currentStatePtr = nullptr;

	// The id of the previous state in the state machine.
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	UStateKey* _previousStateID;
	
	// The object for the previous state in the state machine.
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	UStateBase* _previousStatePtr = nullptr;

	// The id of the next state in the state machine. (Is only relevant during exit.)
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	UStateKey* _nextStateID;
	
	// The object for the next state in the state machine. (Is only relevant during exit.)
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	UStateBase* _nextStatePtr = nullptr;

	// A runtime map that holds the state key to state object structure.
	TMap<UStateKey*, TWeakObjectPtr<UStateBase>> _idToState;

	// Defines the initial state of the state machine.  On startup the state machine
	// will automatically transition into this state.
	UPROPERTY(EditAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	UStateKey* _initialStateID;

	// A structure that declares a mapping between state key and state object.  
	// This will be used at runtime to create the state components.
	UPROPERTY(EditAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	TMap<UStateKey*, TSubclassOf<UStateBase>> _availableStates;

	// If true, this state machine will not raise OnStateChanged when new states are requested
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSuppressStateChangeEvents;

	// If set to true logs will occur whenever a state transition occurs.
	UPROPERTY(EditAnywhere, Category = "State", meta = (DisplayThumbnail = false))
	bool _logTransitions;

	// If set, try to instantiate new states when _idToState returns a garbage-collected object. (Yes, this is bonkers.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bWorkAroundGarbageCollectedStateInstances = true;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Initializes the state machine.
	void Initialize();

public:
	UStateMachine();

	// Registers a new state with the state machine.  Requires both a state key and a state class object.
	void RegisterState(UStateKey* stateID, TSubclassOf<UStateBase> stateClass);

	// Raised when state changes
	FStateChangedEvent OnStateChanged;
	
	// Sets which state should be used as the intial state.  
	// This will only work properly if modified before BeginPlay() fires.
	void SetInitialState(UStateKey* stateID);

	// Modifies whether or not the state machine should log transitions to the console.
	UFUNCTION(BlueprintCallable)
	void SetLogTransitions(bool bLogTransitions);

	// Requests a state transition by passing in a state key.
	UFUNCTION(BlueprintCallable)
	virtual EStateMachineResult Request(UStateKey* stateID) override;

	// Attempts to get a state given a state key.  Returns back a result enum based on the outcome.
	UFUNCTION(BlueprintCallable)
	virtual EStateMachineResult TryGetState(UStateKey* stateID, UStateBase* &state) override;

	// Returns the state key for the current state.
	UFUNCTION(BlueprintCallable)
	virtual UStateKey* GetCurrentID() const override;

	// Returns the current state object.
	UFUNCTION(BlueprintCallable)
	UStateBase* GetCurrentState() const;

	// Returns the state key for the last state.
	UFUNCTION(BlueprintCallable)
	virtual UStateKey* GetPreviousID() const override;

	// Returns the previous state object.
	UFUNCTION(BlueprintCallable)
	UStateBase* GetPreviousState() const;

	// Returns the state key for the next state.
	UFUNCTION(BlueprintCallable)
	virtual UStateKey* GetNextID() const override;

	// Returns the next state object.
	UFUNCTION(BlueprintCallable)
	UStateBase* GetNextState() const;

	// Returns true if the current state key matches the input state key.
	UFUNCTION(BlueprintCallable)
	bool IsCurrentState(UStateKey* stateID) const { return GetCurrentID() == stateID; };

	// Returns true if the previous state key matches the input state key.
	UFUNCTION(BlueprintCallable)
	bool IsPreviousState(UStateKey* stateID) const { return GetPreviousID() == stateID; };

	// Returns true if the next state key matches the input state key.
	UFUNCTION(BlueprintCallable)
	bool IsNextState(UStateKey* stateID) const { return GetNextID() == stateID; };
};

