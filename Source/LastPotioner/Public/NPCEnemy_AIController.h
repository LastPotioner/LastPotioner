// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCEnemy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class LASTPOTIONER_API ANPCEnemy_AIController : public AAIController
{
	GENERATED_BODY()
public: //конструктор 
    explicit ANPCEnemy_AIController(FObjectInitializer const& FObjectInitializer); // инициалезатор обьекта  
    
protected:
    virtual void OnPossess(APawn* InPawn) override;
      	
};
