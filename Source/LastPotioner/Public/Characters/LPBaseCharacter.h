// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "LPBaseCharacter.generated.h"

class ALPWeapon;
class ULPAttributeComponent;
class UAnimMontage;
class USoundBase;

UCLASS()
class LASTPOTIONER_API ALPBaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ALPBaseCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FHitResult& HitResult) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	ULPAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere)
	ALPWeapon* CurrentWeapon;

	void PlayAttackMontage();

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* OneHandedAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* TwoHandedAttackMontage;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(bool CollisionEnabled) const;

	virtual bool CanAttack() const;
	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void OnAttackEnd();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	virtual void PlayDeathAnimMontage();
	virtual void Die();

	virtual void HandleDamage(float DamageAmount);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HitReactMontage;

	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	UParticleSystem* HitParticles;

private:
	float CalculateHitAngle(const FHitResult& HitResult) const;
	void DirectionalHitReact(const FHitResult& HitResult);
};
