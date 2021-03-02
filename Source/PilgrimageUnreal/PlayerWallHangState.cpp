#include "PlayerWallHangState.h"
#include "PlayerPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"

void UPlayerWallHangState::Enter()
{
	PlayerPawn->CapsuleComponent->SetSimulatePhysics(false);
	PlayerPawn->StateFlags.LocomotionState = Wall_Hang;
	PlayerPawn->StateFlags.bMovementAllowed = false;
	PlayerPawn->ResetJumpFlags();
}

void UPlayerWallHangState::Exit()
{
	PlayerPawn->CapsuleComponent->SetSimulatePhysics(true);
	PlayerPawn->StateFlags.bMovementAllowed = true;
}