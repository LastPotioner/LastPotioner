// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LPHealthBarComponent.generated.h"

class ULPHealthBarWidget;

UCLASS()
class LASTPOTIONER_API ULPHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);

private:
	UPROPERTY() // for garbage collector and be sure that variable initialized
	ULPHealthBarWidget* HealthBarWidget;
};
