// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LPInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotsCreatedSignature);

class ALPBaseItem;
class ULPInventorySlot;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LASTPOTIONER_API ULPInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULPInventoryComponent();
	
	UFUNCTION(BlueprintCallable)
	int AddItem(const ALPBaseItem* Item);

	UFUNCTION(BlueprintCallable)
	int AddItemInSlot(const ALPBaseItem* Item, int SlotIndex);

	UFUNCTION(BlueprintCallable)
	int GetSlotsNum() const { return InventoryContainer.Num(); }

	UFUNCTION(BlueprintCallable)
	const ULPInventorySlot* GetSlotByIndex(int Index) const;

	UPROPERTY(BlueprintAssignable)
	FOnSlotsCreatedSignature OnSlotsCreated;

	UFUNCTION(BlueprintCallable)
	void SwapSlots(int FirstIndex, int SecondIndex);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TArray<ULPInventorySlot*> InventoryContainer;

	int FindSuitableSlot(const ALPBaseItem* Item);
};
