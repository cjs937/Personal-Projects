// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerJumpState.h"
#include "Components/CapsuleComponent.h"
#include "PlayerStateMachine.h"
#include "PlayerStates.h"

void UPlayerJumpState::Enter()
{
	UStateBase::Enter();

	PlayerPawn->SetLocomotionState(Jump_Rise);
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
		//If player is grounded
		if (PlayerPawn->IsGrounded())
		{
			OwnerStateMachine->Request(UPlayerIdleState::StaticClass());
		}
		//If player has let go of jump or the jump has passed the max jump startup length
		else if (!PlayerPawn->bJumpInputHeld || GetWorld()->GetTimeSeconds() - JumpStartTime > PlayerPawn->MaxJumpHoldTime)
		{
			OwnerStateMachine->Request(UPlayerFallState::StaticClass());
		}
	}
}