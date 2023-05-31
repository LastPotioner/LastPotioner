// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class ALPCharacter;

USTRUCT(BlueprintType)
struct FInteractionResult
{
	GENERATED_BODY()

	FInteractionResult()
	{
		ObjectiveID = FName("");
		Value = 0;
	}

	FInteractionResult(FName ObjectiveID, int Value)
	{
		this->ObjectiveID = ObjectiveID;
		this->Value = Value;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectiveID = FName("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Value = 0;
};

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};


class LASTPOTIONER_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	FInteractionResult Interact(ALPCharacter* Character);

	UFUNCTION(BlueprintNativeEvent)
	void ToggleToolTipTextVisibility();
};
