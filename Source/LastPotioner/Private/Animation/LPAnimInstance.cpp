// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LPAnimInstance.h"
#include "Characters/LPCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ULPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ALPCharacter>(TryGetPawnOwner());
	if (SlashCharacter)
	{
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
		SlashCharacter->OnCharacterStateChanged.AddUObject(this, &ULPAnimInstance::OnCharacterStateChanged);
	}
}

void ULPAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SlashCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
		
		IsFalling = SlashCharacterMovement->IsFalling();
		IsStanding = SlashCharacterMovement->Velocity.Size() == 0;
	}
}

void ULPAnimInstance::OnCharacterStateChanged(ECharacterState NewState) 
{
	CharacterState = NewState;
}

