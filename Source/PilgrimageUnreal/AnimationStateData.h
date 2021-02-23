// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AnimationStateData.generated.h"

UENUM()
enum ELocomotionState //: uint16
{
	Idle = 0,
	Run,
	Jump_Rise,
	Jump_Idle,
	Land,
	Wall_Cling
};

UCLASS(BlueprintType)
class PILGRIMAGEUNREAL_API UAnimationStateData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ELocomotionState> LocomotionState;
};
