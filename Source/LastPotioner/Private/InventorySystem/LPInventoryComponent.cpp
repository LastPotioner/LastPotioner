// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/LPInventoryComponent.h"
#include "Items/LPBaseItem.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventoryComponent, All, All);

struct FItemSignature;

ULPInventoryComponent::ULPInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULPInventoryComponent::SwapSlots(int SelfSlotIndex, int SourceSlotIndex, ULPInventoryComponent* SourceInventory)
{
	if (!SourceInventory)
	{
		UE_LOG(LogInventoryComponent, Warning, TEXT("SourceInventory is null"));
		return;
	}

	FItemSignature DataBuf;
	DataBuf.CopyFrom(InventoryContainer[SelfSlotIndex]);
	SetSlotByIndex(SelfSlotIndex, SourceInventory->GetSlotByIndex(SourceSlotIndex));
	SourceInventory->SetSlotByIndex(SourceSlotIndex, DataBuf);
	SourceInventory->OnSlotChanged.Broadcast(SourceSlotIndex);
	OnSlotChanged.Broadcast(SelfSlotIndex);
}

const FItemSignature& ULPInventoryComponent::GetSlotByIndex(int Index) const
{
	checkf(Index < InventoryContainer.Num(), TEXT("Index < InventoryContainer.Num()"));
	return InventoryContainer[Index];
}

void ULPInventoryComponent::SetSlotByIndex(int Index, const FItemSignature& NewData)
{
	InventoryContainer[Index].CopyFrom(NewData);
}

TArray<FItemSignature> ULPInventoryComponent::GetSlotsByObjectiveID(FName ObjectiveID) 
{
	TArray<FItemSignature> Result;

	for (const FItemSignature& Item : InventoryContainer)
	{
		if (Item.ObjectiveID == ObjectiveID)
		{
			Result.Add(Item);
		}
	}

	return Result;
}

int ULPInventoryComponent::AddItemByClassRef(const ALPBaseItem* Item)
{
	return AddItem(Item->GetSlotData());
}

int ULPInventoryComponent::AddItem(const FItemSignature& Item)
{
	const int Value = Item.Value;
	int Overage = Value;

	// Until all the items are in the slots 
	while (Overage > 0)
	{
		const int SlotIndex = FindSuitableSlot(Item);

		if (SlotIndex != -1)
		{
			Overage = Value - AddItemInSlot(Item, SlotIndex);
		}
		else
		{
			// TODO: Inventory overflow actions
			return Value - Overage;
		}
	}

	return Value;
}

int ULPInventoryComponent::AddItemInSlot(const FItemSignature& Item, int SlotIndex)
{
	if (Item.Value < 0 || SlotIndex < 0) return 0;

	if (InventoryContainer[SlotIndex].ID == -1)
	{
		InitializeSlot(SlotIndex, Item);
		return Item.Value;
	}
	
	FItemSignature& Slot = InventoryContainer[SlotIndex];

	const int Value = Item.Value;
	if (Item.ID == -1) return 0;
	if (Slot.ID != Item.ID || Slot.Value == Slot.MaxStackSize) return 0;

	const int NewValue = Slot.Value + Value;
	const int Overage = NewValue > Slot.MaxStackSize ? NewValue % Slot.MaxStackSize : 0;
	Slot.Value = NewValue - Overage;
	OnSlotChanged.Broadcast(SlotIndex);
	
	return Value - Overage;
}

int ULPInventoryComponent::FindSuitableSlot(const FItemSignature& Item)
{
	if (Item.ID == -1) return -1;

	int FirstEmptySlot = -1;

	for (int i = 0; i < InventoryContainer.Num(); i++)
	{
		const FItemSignature& Slot = InventoryContainer[i];

		if (Slot.ID == -1 && FirstEmptySlot == -1)
		{
			FirstEmptySlot = i;
		}

		if (Slot.ID == Item.ID && Slot.Value != Slot.MaxStackSize)
		{
			return i;
		}
	}

	return FirstEmptySlot;
}

void ULPInventoryComponent::SubtractSlotValue(int SlotIndex, int Value)
{
	FItemSignature& SlotData = InventoryContainer[SlotIndex];
	SlotData.Value -= Value;
	OnSlotChanged.Broadcast(SlotIndex);

	if (SlotData.Value == 0)
	{
		SetSlotEmpty(SlotIndex);
	}
}

void ULPInventoryComponent::SetSlotEmpty(int SlotIndex)
{
	InventoryContainer[SlotIndex] = FItemSignature();
	OnSlotChanged.Broadcast(SlotIndex);
}

void ULPInventoryComponent::InitializeSlot(int SlotIndex, const FItemSignature& Item)
{
	InventoryContainer[SlotIndex] = Item;
	OnSlotChanged.Broadcast(SlotIndex);
}
