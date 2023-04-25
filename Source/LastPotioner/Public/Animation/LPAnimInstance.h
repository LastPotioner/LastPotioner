// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterTypes.h"
#include "LPAnimInstance.generated.h"

class ALPCharacter;
class UCharacterMovementComponent;

UCLASS()
class LASTPOTIONER_API ULPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	ALPCharacter* LPCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* LPCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsStanding;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterState")
	ECharacterState CharacterState;

	UFUNCTION()
	void OnCharacterStateChanged(ECharacterState NewState);
};
