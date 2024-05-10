// Fill out your copyright notice in the Description page of Project Settings.



#include "BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NPCEnemy_AIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer){

    NodeName = "Find Random Location In NavMesh";

};

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

    //задействия контроллера ии и получаем владельцем компьютер мы можем получить контроллера, который владеет пешкой
    if(auto* const cont = Cast<ANPCEnemy_AIController>(OwnerComp.GetAIOwner())){
        if(auto* const npc = cont->GetPawn()){
            //получаем исходное положение нпс
            auto const Origin = npc->GetActorLocation(); //получаем расположение актера 
            
            //получение навигационой системы и генерация случайном локации
            if(auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld())){
                    FNavLocation Loc;
                    // программа ищет случайную точку в навигационой сети  от точки Orogin в пределе радиуса SearchRadius
                    //и помещать значение в переменую Loc
                    if(NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc)){
                    
                        //из ownerComp мы можем получить копонент для доски,  поэтому надо разоминовать указатель  и задать в виде вектора и получаем значение значение на доске 
                        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),Loc.Location);
                  
                    }
                    
                    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                    return EBTNodeResult::Succeeded;
            }
            
        }
        
    } 
    
    
    return EBTNodeResult::Failed;


   
};
