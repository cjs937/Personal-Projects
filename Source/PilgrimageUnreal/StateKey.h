///////////////////////////////////////////////////////////////////////
//
// StateKey.h
//
// Brief:  
//
// Author:  William Roberts
// Created: 2018/04/05
//
// Schell Games LLC
//
///////////////////////////////////////////////////////////////////////


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StateKey.generated.h"

/**
 * Represents a key for the State Machine to map ID's to State Objects.
 */
UCLASS(Blueprintable)
class PILGRIMAGEUNREAL_API UStateKey : public UDataAsset
{
	GENERATED_BODY()
	
};
