// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PilgrimagePlayerController.generated.h"

class UInputBuffer;

UCLASS()
class PILGRIMAGEUNREAL_API APilgrimagePlayerController : public APlayerController
{
	GENERATED_BODY()
	APilgrimagePlayerController();

	UPROPERTY(EditDefaultsOnly)
	UInputBuffer* InputBuffer;
};
