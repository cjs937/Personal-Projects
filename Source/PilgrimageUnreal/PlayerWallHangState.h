#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "PlayerWallHangState.generated.h"

UCLASS()
class PILGRIMAGEUNREAL_API UPlayerWallHangState : public UPlayerStateBase
{
	GENERATED_BODY()

	UPrimitiveComponent* SnapTarget;

	void Enter() override;
	void Exit() override;
	
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

public:
	void InitWallSnap(const FHitResult& HitData);
};

