#include "PlayerWallHangState.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

void UPlayerWallHangState::Enter()
{
	UPlayerStateBase::Enter();

	PlayerPawn->CapsuleComponent->SetSimulatePhysics(false);
	PlayerPawn->StateFlags.LocomotionState = Wall_Hang;
	PlayerPawn->StateFlags.bMovementAllowed = false;
	PlayerPawn->ResetJumpFlags();
}

void UPlayerWallHangState::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (IsValid(SnapTarget))
	{
		FVector MeshPos = PlayerPawn->SkeletalMesh->GetComponentLocation();
		FRotator LerpRotator = PlayerPawn->SkeletalMesh->GetComponentRotation();
		FVector LookTarget;
		SnapTarget->GetClosestPointOnCollision(MeshPos, LookTarget);
		LookTarget.Z = MeshPos.Z;

		//DrawDebugSphere(GetWorld(), LookTarget, 10.0f, 15, FColor::Blue, true);
		//DrawDebugSphere(GetWorld(), MeshPos, 10.0f, 15, FColor::Blue, true);
		//DrawDebugLine(GetWorld(), LookTarget, MeshPos, FColor::Red, true, -1.0f, (uint8)'\000', 7.0f);

		LerpRotator = UKismetMathLibrary::FindLookAtRotation(MeshPos, LookTarget);

		LerpRotator = UKismetMathLibrary::RInterpTo(PlayerPawn->SkeletalMesh->GetComponentRotation(), LerpRotator, DeltaTime, 500.0f);

		PlayerPawn->SkeletalMesh->SetWorldRotation(LerpRotator, false);
	}
}

void UPlayerWallHangState::Exit()
{
	UPlayerStateBase::Exit();

	PlayerPawn->CapsuleComponent->SetSimulatePhysics(true);
	PlayerPawn->StateFlags.bMovementAllowed = true;
}

void UPlayerWallHangState::InitWallSnap(const FHitResult& HitData)
{
	SnapTarget = HitData.GetComponent();//GetActor();
}