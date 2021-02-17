///////////////////////////////////////////////////////////////////////
//
// EnumUtilityFunction.h
//
// Brief:  Contains utility functions for dealing with enums.
//
// Author:  William Roberts
// Created: 2018/06/22
//
// Schell Games LLC
//
///////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

/**
 * Converts the string name to an enum if possible. ex: GetEnumValueFromString<EControllerHand>("EControllerHand", "Left", hand)
 * @return True if the name was found and converted to a value.
 * @param EnumName The name of the enum type (This should match the value passed into the template)
 * @param outValue This will contain the value of the enum if found. Otherwise, it will default to 0.
 */
template <typename EnumType>
static FORCEINLINE bool GetEnumValueFromString(const FString& enumName, const FString& String, EnumType& outValue)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *enumName, true);

	if (!Enum)
	{
		outValue = EnumType(0);
		return false;
	}

	outValue = (EnumType)Enum->GetIndexByNameString(*String, EGetByNameFlags::None);

	return (uint8)outValue != INDEX_NONE; // enums can only be uint8 in Unreal
}