// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "BaseChest.generated.h"

UCLASS()
class LASTPOTIONER_API ABaseChest : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ABaseChest();

};
