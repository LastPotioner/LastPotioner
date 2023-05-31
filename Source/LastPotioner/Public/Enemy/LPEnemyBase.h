// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/LPBaseCharacter.h"
#include "LPEnemyBase.generated.h"

class ULPHealthBarComponent;
class UPawnSensingComponent;
class ALPWeapon;

UCLASS()
class LASTPOTIONER_API ALPEnemyBase : public ALPBaseCharacter
{
	GENERATED_BODY()

public:
	ALPEnemyBase();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetHit_Implementation(const FHitResult& HitResult) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Death0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	virtual void HandleDamage(float DamageAmount) override;
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void ChaseTarget();
	void StopChase();
	virtual bool CanAttack() const override;
	virtual void FastAttack() override;
	virtual void OnAttackEnd() override;
	virtual void PlayDeathAnimMontage() override;
	virtual void Die() override;

	UFUNCTION(BlueprintImplementableEvent)
	void TransferObjectiveID() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	FName ObjectiveID;
	
	UPROPERTY(EditAnywhere)
	float RotationToTargetSpeed = 2.0f;

	void SetEnemyState(EEnemyState NewState);

	UFUNCTION()
	void OnAnyMontageInterrupted(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALPWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere)
	ULPHealthBarComponent* HealthBarComponent;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	/*
	 * AI
	 */

	UPROPERTY()
	class AAIController* EnemyController;

	bool InTargetRange(const AActor* Target, float Range) const;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void MoveToTarget(const AActor* Target) const;

	void SetHealthBarVisibility(bool Visible) const;

	UPROPERTY(EditAnywhere)
	float ChasingSpeed = 300.0f;

	/*
	 * Patrol
	 */

	void StartPatrolling();

	UPROPERTY(EditAnywhere)
	float PatrollingSpeed = 125.0f;

	FTimerHandle PatrolTimer;

	void PatrolTimerFinished() const;
	void ChangePatrolTarget();

	UPROPERTY(EditAnywhere, Category = "AI")
	float OnTargetReachedWaiting = 2.0f;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	/*
	 * Combat
	 */

	UPROPERTY(VisibleAnywhere)
	APawn* CombatTarget;

	UPROPERTY(EditAnywhere)
	float CombatRadius = 1500.0f;

	void StartAttackTimer();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 150.0f;

	bool IsInsideAttackRadius() const;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackWaitingMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackWaitingMax = 1.0f;

	void LoseInterest();
	void RotateToTarget() const;
};
