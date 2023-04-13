// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/LPInventoryComponent.h"
#include "InventorySystem/LPInventorySlot.h"
#include "Items/LPBaseItem.h"

struct FItemSlotData;

ULPInventoryComponent::ULPInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULPInventoryComponent::SwapSlots(int FirstIndex, int SecondIndex)
{
	UE_LOG(LogTemp, Display, TEXT("Swap1"));
	InventoryContainer[FirstIndex]->SwapWith(InventoryContainer[SecondIndex]);
}

void ULPInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < InventoryContainer.Num(); i++)
	{
		InventoryContainer[i] = NewObject<ULPInventorySlot>();
	}

	OnSlotsCreated.Broadcast();
}


const ULPInventorySlot* ULPInventoryComponent::GetSlotByIndex(int Index) const
{
	if (Index < InventoryContainer.Num())
	{
		return InventoryContainer[Index];
	}

	return nullptr;
}

int ULPInventoryComponent::AddItem(const ALPBaseItem* Item)
{
	const int Value = Item->GetValue();
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

int ULPInventoryComponent::AddItemInSlot(const ALPBaseItem* Item, int SlotIndex)
{
	if (!Item || Item->GetValue() < 0 || SlotIndex < 0) return 0;

	if (InventoryContainer[SlotIndex]->GetItemData().ID == -1)
	{
		InventoryContainer[SlotIndex]->InitializeSlot(Item->GetSlotData());
		return Item->GetValue();
	}

	return InventoryContainer[SlotIndex]->AddItem(Item->GetSlotData());
}

int ULPInventoryComponent::FindSuitableSlot(const ALPBaseItem* Item)
{
	if (!Item) return -1;

	int FirstEmptySlot = -1;
	
	for (int i = 0; i < InventoryContainer.Num(); i++)
	{
		const ULPInventorySlot* Slot = InventoryContainer[i];

		if (Slot->GetItemData().ID == -1 && FirstEmptySlot == -1)
		{
			FirstEmptySlot = i;
		}

		if (Slot->GetItemData().ID == Item->GetID() && !Slot->IsFull())
		{
			return i;
		}
	}

	return FirstEmptySlot;
}
