// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LPAttributeComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LASTPOTIONER_API ULPAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULPAttributeComponent();

	void ReceiveDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAlive() const { return Health > 0; }

	void AddHealth(float HealthDelta);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Acttributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Acttributes")
	float MaxHealth = 100.0f;
};
