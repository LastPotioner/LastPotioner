// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LPBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/LPAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/LPMeleeWeapon.h"
#include "Weapons/LPWeapon.h"

ALPBaseCharacter::ALPBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<ULPAttributeComponent>(TEXT("AttributeComponent"));
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);	
}

void ALPBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALPBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALPBaseCharacter::GetHit_Implementation(const FHitResult& HitResult)
{
	if (!GetWorld()) return;

	UGameplayStatics::PlaySoundAtLocation(this, HitSound, HitResult.Location);
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, HitResult.ImpactPoint,
	                                         HitResult.Normal.Rotation());
	
	if (IsAlive())
	{
		DirectionalHitReact(HitResult);
	}
}

float ALPBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	HandleDamage(DamageAmount);

	return DamageAmount;
}

void ALPBaseCharacter::AddHealth(float Health)
{
	AttributeComponent->AddHealth(Health);
}

bool ALPBaseCharacter::IsAlive() const
{
	return AttributeComponent && AttributeComponent->IsAlive();
}

void ALPBaseCharacter::PlayFastAttackMontage()
{
	if (!CurrentWeapon || (!OneHandedAttackMontage && !TwoHandedAttackMontage)) return;

	UAnimMontage* MontageToPlay = nullptr;
	switch (CurrentWeapon->GetWeaponType())
	{
	case EWeaponType::EWT_OneHanded:
		MontageToPlay = OneHandedAttackMontage;
		break;
	case EWeaponType::EWT_TwoHanded:
		MontageToPlay = TwoHandedAttackMontage;
		break;
	}

	if (!MontageToPlay) return;

	const uint8 AttackNumber = FMath::RandRange(0, MontageToPlay->GetNumSections() - 1);
	const FName SectionName = MontageToPlay->GetSectionName(AttackNumber);
	PlayAnimMontage(MontageToPlay, 1, SectionName);
}

void ALPBaseCharacter::SetWeaponCollisionEnabled(bool CollisionEnabled) const
{
	if (auto MeleeWeapon = Cast<ALPMeleeWeapon>(CurrentWeapon))
	{
		MeleeWeapon->ClearIgnoreActors();
		MeleeWeapon->SetCollisionEnabled(CollisionEnabled);
	}
}

bool ALPBaseCharacter::CanAttack() const
{
	return false;
}

void ALPBaseCharacter::FastAttack()
{
}

void ALPBaseCharacter::OnAttackEnd()
{
}

void ALPBaseCharacter::PlayDeathAnimMontage()
{
}

void ALPBaseCharacter::Die()
{
	PlayDeathAnimMontage();
	GetWorldTimerManager().ClearAllTimersForObject(this);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALPBaseCharacter::HandleDamage(float DamageAmount)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		if (!IsAlive())
		{
			Die();
		}
	}
}

void ALPBaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	if (!HitReactMontage) return;

	PlayAnimMontage(HitReactMontage);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, HitReactMontage);
}

float ALPBaseCharacter::CalculateHitAngle(const FHitResult& HitResult) const
{
	const FVector HitNormal = HitResult.Normal;
	const FVector ForwardVector = GetActorForwardVector();
	const FVector Normal = FVector(HitNormal.X, HitNormal.Y, ForwardVector.Z).GetSafeNormal();

	const float DotProduct = FVector::DotProduct(Normal, ForwardVector);
	float Angle = FMath::Acos(DotProduct);
	Angle = FMath::RadiansToDegrees(Angle);

	const float CrossProductZ = FVector::CrossProduct(Normal, ForwardVector).Z;
	Angle *= CrossProductZ < ForwardVector.Z ? 1.0f : -1.0f;

	return Angle;
}

void ALPBaseCharacter::DirectionalHitReact(const FHitResult& HitResult)
{
	if (!HitReactMontage) return;
	
	const float Angle = CalculateHitAngle(HitResult);
	FName SectionName = FName("FromBack");

	if (FMath::Abs(Angle) <= 45.0f)
	{
		SectionName = FName("FromFront");
	}
	else if (Angle >= 45.0f && Angle <= 135.0f)
	{
		SectionName = FName("FromRight");
	}
	else if (Angle <= -45.0f && Angle >= -135.0f)
	{
		SectionName = FName("FromLeft");
	}

	PlayHitReactMontage(SectionName);
}