// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBuffer.h"

#include "K2Node_GetInputAxisValue.h"

void UInputBuffer::AddInputToBuffer(EInputKey InputKey, EInputType InputType)
{
	switch (InputType)
	{
		case EInputType::Pressed:
			{
				FInputBufferValue NewValue;
				NewValue.Key = InputKey;
				NewValue.InputType = InputType;
				NewValue.LifeTime = KeyPressLifetime;
				NewValue.FramesInBuffer = 0;
				
				Buffer.Add(NewValue);
				
				FButtonState State;
				State.Key = InputKey;
				State.bIsHeld = true;
				State.FramesHeld = 0;
				
				ButtonStates.Add(InputKey, State);
			}
		break;
		case EInputType::Released:
			{
				FButtonState State;
				State.Key = InputKey;
				State.bIsHeld = false;
				State.FramesHeld = 0;
				
				ButtonStates.Add(InputKey, State);
			}
		break;
		default:
		break;
	}
}

void UInputBuffer::GetInputFromJoystick()
{
}

void UInputBuffer::UpdateBuffer()
{}

bool UInputBuffer::WasButtonPressed(EInputKey ButtonKey)
{
	return false;
}

bool UInputBuffer::IsButtonHeld(EInputKey ButtonKey)
{
	return false;
}
