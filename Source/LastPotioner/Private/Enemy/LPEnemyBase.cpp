// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LPEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/LPAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/LowLevelTestAdapter.h"
#include "UI/LPHealthBarComponent.h"
#include "UI/LPHealthBarWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Characters/LPCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Weapons/LPWeapon.h"

ALPEnemyBase::ALPEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,
	                                                     ECollisionResponse::ECR_Block);

	HealthBarComponent = CreateDefaultSubobject<ULPHealthBarComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SightRadius = 4000.0f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.0f);
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ALPEnemyBase::OnSeePawn);
}

void ALPEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AAIController>(GetController());

	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ALPEnemyBase::OnAnyMontageInterrupted);

	SetHealthBarVisibility(false);
	if (AttributeComponent && HealthBarComponent)
	{
		HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}

	UWorld* World = GetWorld();

	if (World && WeaponClass)
	{
		CurrentWeapon = World->SpawnActor<ALPWeapon>(WeaponClass);
		CurrentWeapon->Equip(GetMesh(), this, this);
		CurrentWeapon->Arm();

		switch (CurrentWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_OneHanded:
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			break;
		case EWeaponType::EWT_TwoHanded:
			CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
			break;
		}
	}

	StartPatrolling();
}

void ALPEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState == EEnemyState::EES_Dead) return;

	if (EnemyState != EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

bool ALPEnemyBase::IsInsideAttackRadius() const
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool ALPEnemyBase::CanAttack() const
{
	float AngleRad = PI;
	if (CombatTarget)
	{
		const FVector ToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		AngleRad = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), ToTarget));
	}
	
	return IsAlive() && AngleRad <= PI / 4.0f && IsInsideAttackRadius() && EnemyState != EEnemyState::EES_Attacking
		&& EnemyState != EEnemyState::EES_Engaged;
}

void ALPEnemyBase::RotateToTarget() const
{
	if (!CombatTarget) return;
	
	const FVector ToTarget = CombatTarget->GetActorLocation() - GetActorLocation();
	const FRotator LookAtRotation = ToTarget.Rotation();

	GetCapsuleComponent()->SetWorldRotation(FMath::RInterpTo(GetCapsuleComponent()->GetComponentRotation(),
	                                                         FRotator(0.f, LookAtRotation.Yaw, 0.f),
	                                                         UGameplayStatics::GetWorldDeltaSeconds(this), RotationToTargetSpeed));
}

void ALPEnemyBase::CheckCombatTarget()
{
	if (!CombatTarget) return;

	if (EnemyState != EEnemyState::EES_Engaged)
	{
		RotateToTarget();
	}

	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		LoseInterest();

		if (EnemyState != EEnemyState::EES_Engaged)
		{
			StartPatrolling();
		}
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing
		&& EnemyState != EEnemyState::EES_Engaged)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		ChaseTarget();
	}

	if (GetVelocity().Size() == 0 && EnemyState == EEnemyState::EES_Chasing)
	{
		ChaseTarget();
	}
}

void ALPEnemyBase::SetHealthBarVisibility(bool Visible) const
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(Visible);
	}
}

void ALPEnemyBase::StartPatrolling()
{
	SetEnemyState(EEnemyState::EES_Patrolling);
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void ALPEnemyBase::ChangePatrolTarget()
{
	if (PatrolTargets.Num() == 0) return;

	uint8 Selection = 0;
	while (PatrolTarget == PatrolTargets[Selection])
	{
		Selection = FMath::RandRange(0, PatrolTargets.Num() - 1);
	}

	PatrolTarget = PatrolTargets[Selection];
}

void ALPEnemyBase::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, 200.0f))
	{
		ChangePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &ALPEnemyBase::PatrolTimerFinished,
		                                OnTargetReachedWaiting);
	}
}

void ALPEnemyBase::PatrolTimerFinished() const
{
	MoveToTarget(PatrolTarget);
}

void ALPEnemyBase::StartAttackTimer()
{
	SetEnemyState(EEnemyState::EES_Attacking);
	const float AttackTime = FMath::RandRange(AttackWaitingMin, AttackWaitingMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &ALPEnemyBase::FastAttack, AttackTime);
}

void ALPEnemyBase::LoseInterest()
{
	CombatTarget = nullptr;
	SetHealthBarVisibility(false);
}

void ALPEnemyBase::ChaseTarget()
{
	SetEnemyState(EEnemyState::EES_Chasing);
	PawnSensingComponent->SightRadius = CombatRadius - 100.0f;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

void ALPEnemyBase::StopChase()
{
	CombatTarget = nullptr;
	SetHealthBarVisibility(false);
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	SetEnemyState(EEnemyState::EES_Patrolling);
	CheckPatrolTarget();
}

void ALPEnemyBase::FastAttack()
{
	if (CombatTarget == nullptr) return;

	SetEnemyState(EEnemyState::EES_Engaged);
	PlayFastAttackMontage();
}


void ALPEnemyBase::OnAttackEnd()
{
	SetEnemyState(EEnemyState::EES_NoState);
	CheckCombatTarget();
}

void ALPEnemyBase::OnSeePawn(APawn* Pawn)
{
	if (!Pawn || !Pawn->ActorHasTag("LPCharacter") || EnemyState == EEnemyState::EES_Dead) return;
	
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking;

	if (bShouldChaseTarget)
	{
		CombatTarget = Pawn;
		SetHealthBarVisibility(true);
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		ChaseTarget();
	}
}

void ALPEnemyBase::MoveToTarget(const AActor* Target) const
{
	if (!EnemyController || !Target) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(40.0f);
	EnemyController->MoveTo(MoveRequest);
}

bool ALPEnemyBase::InTargetRange(const AActor* Target, float Range) const
{
	if (!Target) return false;
	const float DistToTarget = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

	return DistToTarget <= Range;
}

void ALPEnemyBase::Die()
{
	Super::Die();

	SetEnemyState(EEnemyState::EES_Dead);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	LoseInterest();
	GetController()->Destroy();
	if (CurrentWeapon)
	{
		CurrentWeapon->SetLifeSpan(5.0f);
	}
	SetLifeSpan(5.0f);
}

void ALPEnemyBase::SetEnemyState(EEnemyState NewState)
{
	if (EnemyState != EEnemyState::EES_Dead)
	{
		EnemyState = NewState;
	}
}

void ALPEnemyBase::OnAnyMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == OneHandedAttackMontage && bInterrupted)
	{
		OnAttackEnd();
	}
}

void ALPEnemyBase::GetHit_Implementation(const FHitResult& HitResult)
{
	Super::GetHit_Implementation(HitResult);
}

void ALPEnemyBase::PlayDeathAnimMontage()
{
	if (!DeathMontage) return;

	const uint8 Selection = FMath::RandRange(0, DeathMontage->GetNumSections() - 1);

	FName SectionName = DeathMontage->GetSectionName(0);
	DeathPose = EDeathPose::EDP_Death0;
	if (Selection <= (int)EDeathPose::EDP_Death2) // prevention of undefined behavior
	{
		DeathPose = static_cast<EDeathPose>(Selection);
		SectionName = DeathMontage->GetSectionName(Selection);
	}

	PlayAnimMontage(DeathMontage);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName);
}

void ALPEnemyBase::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (HealthBarComponent)
	{
		HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}
}

float ALPEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!CombatTarget && EventInstigator && EventInstigator->GetPawn())
	{
		OnSeePawn(EventInstigator->GetPawn());
	}

	return DamageAmount;
}
