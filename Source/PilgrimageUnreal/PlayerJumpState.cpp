// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerJumpState.h"
#include "Components/CapsuleComponent.h"
#include "PlayerStateMachine.h"
#include "PlayerStates.h"

void UPlayerJumpState::Enter()
{
	PlayerPawn->StateFlags.LocomotionState = Jump_Rise;
	FVector CancelledVelocity = PlayerPawn->CapsuleComponent->GetPhysicsLinearVelocity();
	CancelledVelocity.Z = 0.0f;
	PlayerPawn->CapsuleComponent->SetPhysicsLinearVelocity(CancelledVelocity);

	PlayerPawn->CapsuleComponent->AddImpulse(FVector::UpVector * PlayerPawn->JumpForce);
	PlayerPawn->NumJumps++;

	JumpStartTime = GetWorld()->GetTimeSeconds();
}


void UPlayerJumpState::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	//If the minimum jump time has passed
	if (GetWorld()->GetTimeSeconds() - JumpStartTime > PlayerPawn->JumpStartupTime)
	{
		//If player is grounded or has let go of jump
		if (PlayerPawn->IsGrounded() || !PlayerPawn->bJumpInputHeld)
		{
			OwnerStateMachine->Request(UPlayerIdleState::StaticClass());
		}
	}
	//If the jump has passed the max jump startup length
	else if(GetWorld()->GetTimeSeconds() - JumpStartTime > PlayerPawn->MaxJumpHoldTime)
	{
		OwnerStateMachine->Request(UPlayerFallState::StaticClass());
	}
}
