// Fill out your copyright notice in the Description page of Project Settings.




#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}


EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	//poluchaem pozitiu igroka

	if(auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) //poluchaem igroka iz vsego mira u stavim index igroka 0::: static function
	{
		auto const PlayerLocation = Player->GetActorLocation();
		if (SearchRandom) {
			FNavLocation Loc;
			//poluchenie navigacionoy system u sozdanie sluchainoy tochki vokrug igroka
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld())) {
				//poluchaem slychanuy tochki vokrug igroka
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc)) {


					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		//esli poisk ne vedetsa to zapisivaem mestopolojenie igroka
		else {
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}

