#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "PlayerWallHangState.generated.h"

UCLASS()
class PILGRIMAGEUNREAL_API UPlayerWallHangState : public UPlayerStateBase
{
	GENERATED_BODY()

	void Enter() override;
	void Exit() override;
};

