// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"
#include "NPCEnemy_AIController.h"
#include "NPCEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer):
UBTTask_BlackboardBase{ObjectInitializer}{

    NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{   //poluchaem ai controller
    if(auto* const Cont = Cast<ANPCEnemy_AIController>(OwnerComp.GetAIOwner())){
        //poluchaem npc
        if(auto* const NPC = Cast<ANPCEnemy>(Cont->GetPawn()))
        {   //poluchaem blackboard
            if(auto* const BC = OwnerComp.GetBlackboardComponent()){
                //poluchemie nomera patrylux tochek e min e max indices
                auto const NoOfPoints = NPC->GetPatrolPath()->Num();
                auto const MinIndex = 0;
                auto const MaxIndex = NoOfPoints - 1;
                auto Index = BC->GetValueAsInt(GetSelectedBlackboardKey());
                
                //change direction if we are at the first or last index if we are in bidirection mode
                if(bBiDirectional){
                    if(Index >= MaxIndex && Direction == EDirectionType::Forward){
                        Direction = EDirectionType::Reverse;
                        
                    }else if(Index == MinIndex && Direction == EDirectionType::Reverse){
                        Direction = EDirectionType::Forward;
                    }
                }
                
                // write new value of index to blackboard
                BC->SetValueAsInt(GetSelectedBlackboardKey(),
                (Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);
                
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                return EBTNodeResult::Succeeded;
            }
        }
    }
    return EBTNodeResult::Failed;
}
