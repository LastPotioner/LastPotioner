// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCEnemy_AIController.h"
#include "NPCEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Characters/LPCharacter.h"

ANPCEnemy_AIController:: ANPCEnemy_AIController(FObjectInitializer const& ObjectInitializer)
{
    SetupPerceptionSystem();
}

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

void ANPCEnemy_AIController::SetupPerceptionSystem(){

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));//konfiguracia zrenia
    if(SightConfig){ //esli configuracia ne null
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
        TEXT("Perception Component")));
        
        SightConfig->SightRadius = 500.f;//opredelenia radiusa zrenia
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
        SightConfig->PeripheralVisionAngleDegrees = 90.f;
        SightConfig->SetMaxAge(5.f);
        SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        
        
        GetPerceptionComponent()->SetDominantSense(*SightConfig->
        GetSenseImplementation());
        GetPerceptionComponent()-> OnTargetPerceptionUpdated.AddDynamic(this, &ANPCEnemy_AIController::OnTargetDetected);
        GetPerceptionComponent()->ConfigureSense((*SightConfig));  

            }

}
void ANPCEnemy_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus){


    if(auto* const ch = Cast<ALPCharacter>(Actor))
    {
            GetBlackboardComponent()->SetValueAsBool("CanSeePLayer", Stimulus.WasSuccessfullySensed());
   
    }
}