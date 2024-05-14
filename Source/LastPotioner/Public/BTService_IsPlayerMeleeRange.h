// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPlayerMeleeRange.generated.h"

/**
 * 
 */
UCLASS()
class LASTPOTIONER_API UBTService_IsPlayerMeleeRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
    UBTService_IsPlayerMeleeRange();
    void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
    float MeleeRange = 2.5f;
	
};
