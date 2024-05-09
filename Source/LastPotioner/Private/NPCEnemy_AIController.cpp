// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCEnemy_AIController.h"
#include "NPCEnemy.h"

ANPCEnemy_AIController:: ANPCEnemy_AIController(FObjectInitializer const& ObjectInitializer)
{}

void ANPCEnemy_AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    if(ANPCEnemy* const npc = Cast<ANPCEnemy>(InPawn)){
        if(UBehaviorTree* const tree = npc -> GetBehaviorTree()) // получаем дерево 
        {
            UBlackboardComponent* b;
            UseBlackboard(tree -> BlackboardAsset, b); // ии использует ресурс blackboard, пытается достать обьект из blackboard, если его нет то создает его 
            Blackboard = b;
            RunBehaviorTree(tree); // запускаем дерево 
            
            
        }
    }
}