// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
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
    
private:
       //vospriyatie mesta npc
    class UAISenseConfig_Sight* SightConfig;
    
     void SetupPerceptionSystem();

    UFUNCTION()
    void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

    
      	
};
