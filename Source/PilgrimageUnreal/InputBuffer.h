// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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

class PILGRIMAGEUNREAL_API InputBuffer
{
public:
	InputBuffer();
	~InputBuffer();
};
