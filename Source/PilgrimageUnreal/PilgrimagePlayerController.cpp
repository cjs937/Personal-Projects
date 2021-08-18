// Fill out your copyright notice in the Description page of Project Settings.


#include "PilgrimagePlayerController.h"
#include "InputBuffer.h"

APilgrimagePlayerController::APilgrimagePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	InputBuffer = CreateDefaultSubobject<UInputBuffer>("InputBuffer");
}

