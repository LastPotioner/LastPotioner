// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/LPHealthBarComponent.h"
#include "Components/ProgressBar.h"
#include "UI/LPHealthBarWidget.h"

void  ULPHealthBarComponent::SetHealthPercent(float Percent)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<ULPHealthBarWidget>(GetUserWidgetObject());
	}

	HealthBarWidget->HealthBar->SetPercent(Percent);
}
