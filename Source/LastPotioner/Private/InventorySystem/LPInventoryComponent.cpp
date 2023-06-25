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

bool ULPInventoryComponent::ContainsRequiredAmountOfItem(TSubclassOf<ALPBaseItem> ItemClass, int Quantity) const
{
	int FoundQuantity = 0;
	for (const auto& Item : InventoryContainer)
	{
		if (Item.ItemClass == ItemClass)
		{
			FoundQuantity += Item.Quantity;

			if (FoundQuantity >= Quantity)
			{
				return true;
			}
		}
	}

	return false;
}

TArray<int> ULPInventoryComponent::GetItemIndexes(TSubclassOf<ALPBaseItem> ItemClass) const
{
	TArray<int> Indexes;

	for (int i = 0; i < InventoryContainer.Num(); i++)
	{
		if (InventoryContainer[i].ItemClass == ItemClass)
		{
			Indexes.Add(i);
		}
	}

	return Indexes;
}

int ULPInventoryComponent::AddItemByRef(ALPBaseItem* Item)
{
	return AddItem(Item->GetItemSignature());
}

int ULPInventoryComponent::AddItem(FItemSignature& Item)
{
	// Until all the items are in the slots
	const int OriginalQuantity = Item.Quantity;
	while (Item.Quantity > 0)
	{
		const int SlotIndex = FindSuitableSlot(Item);

		if (SlotIndex != -1)
		{
			AddItemInSlot(Item, SlotIndex);
		}
		else
		{
			// TODO: Inventory overflow actions
			return OriginalQuantity - Item.Quantity;
		}
	}

	return OriginalQuantity - Item.Quantity;
}

int ULPInventoryComponent::AddItemInSlot(FItemSignature& Item, int SlotIndex)
{
	if (Item.Quantity < 0 || SlotIndex < 0) return 0;

	if (InventoryContainer[SlotIndex].ID == -1)
	{
		InitializeSlot(SlotIndex, Item);
		Item.Quantity = 0;
		return Item.Quantity;
	}
	
	FItemSignature& Slot = InventoryContainer[SlotIndex];

	const int OriginalQuantity = Item.Quantity;
	if (Item.ID == -1) return 0;
	if (Slot.ID != Item.ID || Slot.Quantity == Slot.MaxStackSize) return 0;

	const int NewValue = Slot.Quantity + OriginalQuantity;
	const int Overage = NewValue > Slot.MaxStackSize ? NewValue % Slot.MaxStackSize : 0;
	Item.Quantity = Overage;
	Slot.Quantity = NewValue - Overage;
	OnSlotChanged.Broadcast(SlotIndex);
	
	return OriginalQuantity - Overage;
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

		if (Slot.ID == Item.ID && Slot.Quantity != Slot.MaxStackSize)
		{
			return i;
		}
	}

	return FirstEmptySlot;
}

void ULPInventoryComponent::SubtractSlotValue(int SlotIndex, int Value)
{
	FItemSignature& SlotData = InventoryContainer[SlotIndex];
	SlotData.Quantity -= Value;
	OnSlotChanged.Broadcast(SlotIndex);

	if (SlotData.Quantity == 0)
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
