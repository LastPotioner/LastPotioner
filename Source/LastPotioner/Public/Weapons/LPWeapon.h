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
	EWT_Bow UMETA(DisplayName = "Bow"),
};

class USoundBase;

UCLASS()
class LASTPOTIONER_API ALPWeapon : public ALPBaseItem
{
	GENERATED_BODY()

public:
	ALPWeapon();

	void Equip(USceneComponent* InParent, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const;
	void Arm() const;
	void Disarm() const;
	virtual FInteractionResult Interact_Implementation(ALPCharacter* Character) override;
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	float Damage = 33;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;

	TMap<EWeaponType, FName> ArmSocketNames {
		{EWeaponType::EWT_OneHanded, FName("RightHandSocket")},
		{EWeaponType::EWT_TwoHanded, FName{"TwoHandedArmedWeaponSocket"}},};
		 // Сюда добавить сопоставление EWT_Bow - название сокета куда будет попадать лук при взятии

	TMap<EWeaponType, FName> DisarmSocketNames {
			{EWeaponType::EWT_OneHanded, FName("OneHandedDisarmedWeaponSocket")},
			{EWeaponType::EWT_TwoHanded, FName{"TwoHandedDisarmedWeaponSocket"}}};
	// Сюда добавить сопоставление EWT_Bow - название сокета куда будет попадать лук при убирании
};
