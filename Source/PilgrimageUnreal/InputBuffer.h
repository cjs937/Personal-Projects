// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputBuffer.generated.h"

UENUM(BlueprintType)
enum EInputKey
{
	X,
	Y,
	B,
	A,
	L_Bumper,
	L_Trigger,
	R_Bumper,
	R_Trigger,
	Pause,

	//Joystick states
	Neutral,
	Left,
	Up_Left,
	Up,
	Up_Right,
	Right,
	Down_Right,
	Down,
	Down_Left	
};

UENUM(BlueprintType)
enum EInputType
{
	Pressed,
	Held,
	Released
};

USTRUCT(BlueprintType)
struct PILGRIMAGEUNREAL_API FButtonState
{
	GENERATED_BODY()
	TEnumAsByte<EInputKey> Key;
	bool bIsHeld;
	int FramesHeld;
};

USTRUCT(BlueprintType)
struct PILGRIMAGEUNREAL_API FInputBufferValue
{
	GENERATED_BODY()
	TEnumAsByte<EInputKey> Key;
	TEnumAsByte<EInputType> InputType;
	int LifeTime;
	int FramesInBuffer;
};

UCLASS(BlueprintType)
class PILGRIMAGEUNREAL_API UInputBuffer : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int KeyPressLifetime;

	UPROPERTY(Transient)
	TEnumAsByte<EInputKey> PreviousJoystickPosition;
	
	UPROPERTY(Transient)
	TArray<FInputBufferValue> Buffer;
	
	UPROPERTY(Transient)
	TMap<TEnumAsByte<EInputKey>, FButtonState> ButtonStates;

	UFUNCTION(BlueprintCallable)
	void AddInputToBuffer(EInputKey InputKey, EInputType InputType);
	UFUNCTION(BlueprintCallable)
	void GetInputFromJoystick();
	UFUNCTION(BlueprintCallable)
	void UpdateBuffer();
	UFUNCTION(BlueprintCallable)
	bool WasButtonPressed(EInputKey ButtonKey);
	UFUNCTION(BlueprintCallable)
	bool IsButtonHeld(EInputKey ButtonKey);
};
