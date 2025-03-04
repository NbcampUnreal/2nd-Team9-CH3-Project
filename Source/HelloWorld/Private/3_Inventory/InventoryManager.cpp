// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/InventoryManager.h"

#include "IDetailTreeNode.h"
#include "3_Inventory/Weapon.h"
#include "3_Inventory/WeaponParts.h"

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

TArray<UWeaponParts*> UInventoryManager::GetWeaponParts(FName WeaponName)
{
	TArray<UWeaponParts*> Parts;
	FString WeaponNameStr = WeaponName.ToString(); 
	for (UItemBase* Item : Inventory)
	{
		if (Item->GetItemType() == EItemType::Parts && Item->GetItemName().ToString().Contains(WeaponNameStr))
		{
			Parts.Add(Cast<UWeaponParts>(Item));
		}
	}
	return Parts;
}
