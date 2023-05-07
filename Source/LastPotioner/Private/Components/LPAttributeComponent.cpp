// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LPAttributeComponent.h"

ULPAttributeComponent::ULPAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULPAttributeComponent::AddHealth(float HealthDelta)
{
	Health = FMath::Clamp(Health + HealthDelta, 0, MaxHealth);
}

void ULPAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void ULPAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
}