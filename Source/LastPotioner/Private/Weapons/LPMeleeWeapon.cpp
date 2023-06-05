// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LPMeleeWeapon.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

ALPMeleeWeapon::ALPMeleeWeapon()
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
}

void ALPMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ALPMeleeWeapon::OnBoxBeginOverlap);
}

void ALPMeleeWeapon::ClearIgnoreActors()
{
	IgnoreActors.Empty();
}

void ALPMeleeWeapon::SetCollisionEnabled(bool bCollisionEnabled) const
{
	const ECollisionEnabled::Type CollisionEnabled = bCollisionEnabled
											   ? ECollisionEnabled::QueryOnly
											   : ECollisionEnabled::NoCollision;

	WeaponBox->SetCollisionEnabled(CollisionEnabled);
}

void ALPMeleeWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ALPMeleeWeapon::TryToTakeHit(const FHitResult& HitResult) const
{
	if (const IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor()))
	{
		HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult);
	}
}
