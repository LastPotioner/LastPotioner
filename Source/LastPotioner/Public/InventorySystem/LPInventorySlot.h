// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/LPBaseItem.h"
#include "LPInventorySlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotChangedSignature);

UCLASS(BlueprintType)
class LASTPOTIONER_API ULPInventorySlot : public UObject
{
	GENERATED_BODY()

public:
	ULPInventorySlot() {}

	void InitializeSlot(const FItemSlotData& Item);

	UFUNCTION(BlueprintCallable)
	int GetValue() const { return ItemData.Value; }

	UFUNCTION(BlueprintCallable)
	int AddItem(const FItemSlotData& Item);

	UFUNCTION(BlueprintCallable)
	bool IsFull() const { return ItemData.Value == ItemData.MaxStackSize; }

	UFUNCTION(BlueprintCallable)
	const FItemSlotData& GetItemData() const { return ItemData; }

	UFUNCTION(BlueprintCallable)
	void SetItemData(const FItemSlotData& NewItemData) { ItemData = NewItemData; }

	UFUNCTION(BlueprintCallable)
	void SubtractValue(int Value);

	UPROPERTY(BlueprintAssignable)
	FOnSlotChangedSignature OnSlotChanged;

	UFUNCTION(BlueprintCallable)
	void SwapWith(ULPInventorySlot* SlotForSwap);

private:
	UPROPERTY(VisibleAnywhere)
	FItemSlotData ItemData;

	void SetSlotEmpty();
};

