// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LPWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Interfaces/HitInterface.h"

ALPWeapon::ALPWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(ItemMesh);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Trace Start"));
	BoxTraceStart->SetupAttachment(ItemMesh);

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Trace End"));
	BoxTraceEnd->SetupAttachment(ItemMesh);

	ItemMesh->SetSimulatePhysics(false);
}

void ALPWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ALPWeapon::OnBoxBeginOverlap);
}

void ALPWeapon::TryToTakeHit(const FHitResult& HitResult) const
{
	if (const IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor()))
	{
		HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult);
	}
}

void ALPWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	FVector TraceStart = BoxTraceStart->GetComponentLocation();
	FVector TraceEnd = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.Add(Actor);
	}

	FHitResult HitResult;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		TraceStart,
		TraceEnd,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	
	if (AActor* HitActor = HitResult.GetActor())
	{
		IgnoreActors.AddUnique(HitResult.GetActor());
		UE_LOG(LogTemp, Display, TEXT("%s"), *HitActor->GetName());
		CreateField(HitResult.ImpactPoint);
		UGameplayStatics::ApplyDamage(HitActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		TryToTakeHit(HitResult);
	}
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

void ALPWeapon::SetCollisionEnabled(bool bCollisionEnabled) const
{
	const ECollisionEnabled::Type CollisionEnabled = bCollisionEnabled
		                                           ? ECollisionEnabled::QueryOnly
		                                           : ECollisionEnabled::NoCollision;

	WeaponBox->SetCollisionEnabled(CollisionEnabled);
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

void ALPWeapon::ClearIgnoreActors()
{
	IgnoreActors.Empty();
}

void ALPWeapon::Interact_Implementation(ALPCharacter* Character)
{
}
