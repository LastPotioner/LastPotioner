// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/LPInventorySlot.h"

int ULPInventorySlot::AddItem(const FItemSlotData& Item)
{
	const int Value = Item.Value;
	if (Item.ID == -1) return 0;
	if (ItemData.ID != Item.ID || ItemData.Value == ItemData.MaxStackSize) return 0;

	const int NewValue = ItemData.Value + Value;
	const int Overage = NewValue > ItemData.MaxStackSize ? NewValue % ItemData.MaxStackSize : 0;
	ItemData.Value = NewValue - Overage;
	OnSlotChanged.Broadcast();
	return Value - Overage;
}

void ULPInventorySlot::SubtractValue(int Value)
{
	ItemData.Value -= Value;
	
	if (ItemData.Value == 0)
	{
		SetSlotEmpty();
	}

	OnSlotChanged.Broadcast();
}

void ULPInventorySlot::SwapWith(ULPInventorySlot* SlotForSwap)
{
	const FItemSlotData DataBuf = ItemData;
	ItemData = SlotForSwap->GetItemData();
	SlotForSwap->SetItemData(DataBuf);
	
	OnSlotChanged.Broadcast();
	SlotForSwap->OnSlotChanged.Broadcast();
}

void ULPInventorySlot::SetSlotEmpty()
{
	ItemData = FItemSlotData();
}

void ULPInventorySlot::InitializeSlot(const FItemSlotData& Item)
{
	ItemData = Item;
	OnSlotChanged.Broadcast();
}
