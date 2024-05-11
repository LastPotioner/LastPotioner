// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolPath.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NPCEnemy.generated.h"

UCLASS()
class LASTPOTIONER_API ANPCEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCEnemy();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UBehaviorTree* GetBehaviorTree() const; //функция получения дерева
	
	APatrolPath* GetPatrolPath() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree; //дерево поведения 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	APatrolPath* PatrolPath;

};
