// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerMeleeRange.h"
#include "NPCEnemy_AIController.h"
#include "NPCEnemy.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h" 
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTService_IsPlayerMeleeRange::UBTService_IsPlayerMeleeRange(){
    bNotifyBecomeRelevant = true;
    NodeName = TEXT("Is Player In Melee Range");
}
void UBTService_IsPlayerMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
    auto const * const cont = Cast<ANPCEnemy_AIController>(OwnerComp.GetAIOwner());
    auto const * const npc = Cast<ANPCEnemy>(cont->GetPawn());
    
    auto const * const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(
        GetSelectedBlackboardKey(),
        npc->GetDistanceTo(player)<=MeleeRange
        );
    
}