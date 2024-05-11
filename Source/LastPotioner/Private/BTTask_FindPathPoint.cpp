// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "NPCEnemy.h"
#include "NPCEnemy_AIController.h"
#include "BehaviorTree/blackboardComponent.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer){
NodeName = TEXT("Find PAth Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

    //получаем нпс контроллер
    
    if(auto* const cont = Cast<ANPCEnemy_AIController>(OwnerComp.GetAIOwner())){
        //получение компонента blackboard из  дерева поведния
        if(auto* const bc = OwnerComp.GetBlackboardComponent()){
        //получения индекса из блэкборда
            auto const index = bc->GetValueAsInt((GetSelectedBlackboardKey()));
        
            if(auto* npc = Cast<ANPCEnemy>(cont->GetPawn())) {
                //получение тякущего вектора патрулирования
                auto const Point = npc->GetPatrolPath()->GetPatrolPoint(index);
                auto const GlobalPoint = npc->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
                bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                return EBTNodeResult::Succeeded;
            }
        } 
    }
    
    return EBTNodeResult::Failed;
}
