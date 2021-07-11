// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackData.generated.h"

class UAnimMontage;

UCLASS()
class PILGRIMAGEUNREAL_API UAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PushbackForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockupForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AttackDisplacementForces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachToSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ScaleOverride = FVector::OneVector;
};
