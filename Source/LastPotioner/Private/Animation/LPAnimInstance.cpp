// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LPAnimInstance.h"
#include "Characters/LPCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ULPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	LPCharacter = Cast<ALPCharacter>(TryGetPawnOwner());
	if (LPCharacter)
	{
		LPCharacterMovement = LPCharacter->GetCharacterMovement();
		LPCharacter->OnCharacterStateChanged.AddUObject(this, &ULPAnimInstance::OnCharacterStateChanged);
	}
}

void ULPAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (LPCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(LPCharacterMovement->Velocity);
		
		IsFalling = LPCharacterMovement->IsFalling();
		IsStanding = LPCharacterMovement->Velocity.Size() == 0;
	}
}

void ULPAnimInstance::OnCharacterStateChanged(ECharacterState NewState) 
{
	CharacterState = NewState;
}

