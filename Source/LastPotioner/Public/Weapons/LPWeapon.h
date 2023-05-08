// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/LPBaseItem.h"
#include "LPWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_OneHanded UMETA(DisplayName = "OneHanded"),
	EWT_TwoHanded UMETA(DisplayName = "TwoHanded"),
};

class UBoxComponent;
class USoundBase;

UCLASS()
class LASTPOTIONER_API ALPWeapon : public ALPBaseItem
{
	GENERATED_BODY()

public:
	ALPWeapon();

	void Equip(USceneComponent* InParent, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const;
	void SetCollisionEnabled(bool bCollisionEnabled) const;
	void Arm() const;
	void Disarm() const;
	void ClearIgnoreActors();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						   const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 33;

private:
	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* WeaponBox;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;

	TMap<EWeaponType, FName> ArmSocketNames {
		{EWeaponType::EWT_OneHanded, FName("RightHandSocket")},
		{EWeaponType::EWT_TwoHanded, FName{"TwoHandedArmedWeaponSocket"}}};

	TMap<EWeaponType, FName> DisarmSocketNames {
			{EWeaponType::EWT_OneHanded, FName("OneHandedDisarmedWeaponSocket")},
			{EWeaponType::EWT_TwoHanded, FName{"TwoHandedDisarmedWeaponSocket"}}};

	void TryToTakeHit(const FHitResult& HitResult) const;

public:
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
};
