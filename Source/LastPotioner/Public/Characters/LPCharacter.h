// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LPBaseCharacter.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Characters/CharacterTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/LPAttributeComponent.h"
#include "LPCharacter.generated.h"

class IInteractable;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class ULPAttributeComponent;
class ULPInventoryComponent;
class ALPBaseItem;
class ALPWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDiedSignature);

UCLASS()
class LASTPOTIONER_API ALPCharacter : public ALPBaseCharacter
{
	GENERATED_BODY()

public:
	ALPCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FOnCharacterStateChangedSignature OnCharacterStateChanged;

	virtual void GetHit_Implementation(const FHitResult& HitResult) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	FInteractionResult Interact();

	int AddItemToInventory(ALPBaseItem* Item) const;

	UFUNCTION(BlueprintCallable)
	void SetOpenedStorage(AActor* Storage);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() { return AttributeComponent->GetHealthPercent(); }

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDiedSignature OnCharacterDied;

protected:
	UPROPERTY(BlueprintReadOnly)
	AActor* OpenedStorage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ULPInventoryComponent* InventoryComponent;

	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PickUpOverlappingItem();
	virtual bool CanAttack() const override;
	virtual void FastAttack() override;
	virtual void OnAttackEnd() override;
	void HandleWeaponAction();
	void DisarmWeapon();

	UFUNCTION(BlueprintCallable)
	void OnDisarmEnd();

	UFUNCTION(BlueprintCallable)
	void OnArmEnd();

	void ArmWeapon();

	UFUNCTION(BlueprintNativeEvent)
	void HandleEscape();

	UFUNCTION()
	void StrongAttack();

	UFUNCTION()
	void JumpAttack();

	virtual void Die() override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxMovementSpeed = 600.0f;

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm1;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	int8 CurrentWeaponPosition = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<ALPWeapon*> EquippedWeapons;

	UFUNCTION(BlueprintCallable)
	void ToggleCurrentWeaponVisibility() const;

	/*
	 * Enhanced input actions and their parameters
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* LPMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxWalkSpeed = 600.0f;

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
	UInputAction* StrongAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EscapeAction;

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

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* StrongAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* JumpAttackMontage;

	void CheckForInteractables();

public:
	FORCEINLINE void SetOverlappingItem(ALPBaseItem* Item) { OverlappingItem = Item; }
};
