// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LPWeapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

ALPWeapon::ALPWeapon()
{
	ItemMesh->SetSimulatePhysics(false);
}

void ALPWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ALPWeapon::Equip(USceneComponent* InParent, AActor* NewOwner, APawn* NewInstigator)
{
	if (InParent)
	{
		const FName InSocketName = DisarmSocketNames[WeaponType];
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttachMeshToSocket(InParent, InSocketName);
		SetItemState(EItemState::EIS_Taken);
		SetOwner(NewOwner);
		SetInstigator(NewInstigator);
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}

void ALPWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const
{
	if (InParent)
	{
		ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetIncludingScale, InSocketName);
	}
}

void ALPWeapon::Arm() const
{
	if (!GetOwner()) return;
	const ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());

	if (!CharacterOwner) return;
	USceneComponent* InParent = CharacterOwner->GetMesh();

	const FName InSocketName = ArmSocketNames[WeaponType];
	AttachMeshToSocket(InParent, InSocketName);
}

void ALPWeapon::Disarm() const
{
	if (!GetOwner()) return;
	const ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());

	if (!CharacterOwner) return;
	USceneComponent* InParent = CharacterOwner->GetMesh();

	const FName InSocketName = DisarmSocketNames[WeaponType];
	AttachMeshToSocket(InParent, InSocketName);
}

FInteractionResult ALPWeapon::Interact_Implementation(ALPCharacter* Character)
{
	return FInteractionResult();
}
