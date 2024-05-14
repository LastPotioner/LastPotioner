// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "NPCEnemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class LASTPOTIONER_API UBTService_MeleeAttack : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
	public:
	    UBTTask_MeleeAttack();
	    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent&OwnerComp, uint8* NodeMemory)override;
	private:
	    bool MontageHasFinished(ANPCEnemy* const npc);
	
};
