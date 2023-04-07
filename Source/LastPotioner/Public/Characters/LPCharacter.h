// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LPBaseCharacter.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Characters/CharacterTypes.h"
#include "LPCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class ULPAttributeComponent;
class ALPBaseItem;
class ALPWeapon;

UCLASS()
class LASTPOTIONER_API ALPCharacter : public ALPBaseCharacter
{
	GENERATED_BODY()
public:
	ALPCharacter();
	
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FOnCharacterStateChangedSignature OnCharacterStateChanged;

	virtual void GetHit_Implementation(const FHitResult& HitResult) override;

protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PickUpOverlappingItem();
	virtual bool CanAttack() const override;
	virtual void Attack() override;
	virtual void AttackEnd() override;
	void ArmWeapon();
	void DisarmWeapon();

	UFUNCTION(BlueprintCallable)
	void DisarmEnd();

	UFUNCTION(BlueprintCallable)
	void ArmStart();

	void ContinueArm();

private:
	UPROPERTY(VisibleAnywhere)
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm1;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	int8 CurrentWeaponPosition = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<ALPWeapon*> EquippedWeapons;

	/*
	 * Enhanced input actions and their parameters
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* LPMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	float RotationSensitivityMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PickUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;

	/*
	 * Appearance
	 */

	UPROPERTY(VisibleInstanceOnly)
	ALPBaseItem* OverlappingItem;

	/*
	 * Animation
	 */
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

public:
	FORCEINLINE void SetOverlappingItem(ALPBaseItem* Item) { OverlappingItem = Item; }
};
