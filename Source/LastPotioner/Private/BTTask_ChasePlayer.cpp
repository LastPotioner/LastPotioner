// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"

#include "NPCEnemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer) : 
                                                                                      UBTTask_BlackboardBase{ObjectInitializer}
{
NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
    //poluchaem controller ot npc 
    if(auto* const cont = Cast<ANPCEnemy_AIController>(OwnerComp.GetAIOwner()))
    {    //poluchaem metopolojeniya igroka s blackboard
        auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
       
       //peremeshaemsya k mestopolojeniy igroka
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);
        //zavershaem s uspehom
        FinishLatentTask(OwnerComp, EBTNodeResult:: Succeeded);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}

