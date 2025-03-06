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
				NewItem->AddToRoot();
				break;
			case EItemType::Parts:
				NewItem = NewObject<UWeaponParts>();
				NewItem->AddToRoot();
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

TArray<UWeaponParts*> UInventoryManager::GetWeaponParts(EWeaponType WeaponType)
{
	TArray<UWeaponParts*> PartsArray;
	for (UItemBase* Item : Inventory)
	{
		// 아이템이 Parts 타입일 때
		if (Item->GetItemType() == EItemType::Parts)
		{
			UWeaponParts* Parts = Cast<UWeaponParts>(Item);
			// 입력받은 Weapon타입이고 가지고 있을 떄
			if (Parts->GetWeaponType() == WeaponType && Parts->GetIsOwned())
			{
				PartsArray.Add(Cast<UWeaponParts>(Item));	
			}
		}
	}
	return PartsArray;
}
