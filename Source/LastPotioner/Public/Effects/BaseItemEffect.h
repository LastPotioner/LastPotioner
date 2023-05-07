// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItemEffect.generated.h"

class ALPCharacter;
UCLASS(EditInlineNew, Blueprintable)
class LASTPOTIONER_API UBaseItemEffect : public UObject
{
	GENERATED_BODY()
	
public:	
	UBaseItemEffect();
	UBaseItemEffect(ALPCharacter* CharacterRef);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CreateEffect();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	ALPCharacter* Character;
};
