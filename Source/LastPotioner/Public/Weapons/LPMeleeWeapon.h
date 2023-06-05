// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/LPWeapon.h"
#include "LPMeleeWeapon.generated.h"

class UBoxComponent;

UCLASS()
class LASTPOTIONER_API ALPMeleeWeapon : public ALPWeapon
{
	GENERATED_BODY()

public:
	ALPMeleeWeapon();
	
	void ClearIgnoreActors();
	void SetCollisionEnabled(bool bCollisionEnabled) const;

protected:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                       const FHitResult& SweepResult);

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	void TryToTakeHit(const FHitResult& HitResult) const;
};
