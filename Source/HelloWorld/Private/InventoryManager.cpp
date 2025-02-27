// Copyright Epic Games, Inc. All Rights Reserved.


#include "InventoryManager.h"

#include "IDetailTreeNode.h"
#include "Weapon.h"
#include "WeaponParts.h"

UInventoryManager* UInventoryManager::Instance = nullptr;

UInventoryManager* UInventoryManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UInventoryManager>();
	}
	return Instance;
}

void UInventoryManager::InitializeInventoryFromDataTable(const UDataTable* ItemDataTable)
{
	if (ItemDataTable)
	{
		TArray<FItem*> RowArray;
		ItemDataTable->GetAllRows<FItem>(TEXT("ItemDataTable"), RowArray);

		for (const FItem* Item : RowArray)
		{
			UItemBase* NewItem = nullptr;
			switch (Item->ItemType)
			{
			case EItemType::Weapon:
				NewItem = NewObject<UWeapon>();
				break;
			case EItemType::Parts:
				NewItem = NewObject<UWeaponParts>();
				break;
			default:
				continue;
			}

			if (NewItem)
			{
				NewItem->InitializeItem(*Item);
				// 인벤토리에 추가
				Inventory.Add(NewItem);
			}
		}
	}
}

UItemBase* UInventoryManager::GetItemFromID(const FName ItemID)
{
	for (UItemBase* Item : Inventory)
	{
		if (Item->GetItemID() == ItemID)
		{
			return Item;
		}
	}
	return nullptr;
}