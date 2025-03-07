// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/ItemBase.h"

UItemBase::UItemBase()
{
	ItemData = FItem();
	bIsOwned = false;
}

FName UItemBase::GetItemID() const
{
	return ItemData.ItemID;
}


FName UItemBase::GetItemName() const
{
	return ItemData.ItemName;
}

EItemType UItemBase::GetItemType() const
{
	return ItemData.ItemType;
}

TSoftObjectPtr<UTexture2D> UItemBase::GetItemThumbnail() const
{
	return ItemData.ItemThumbnail;
}

TSoftObjectPtr<UMaterial> UItemBase::GetItemMaterial() const
{
	return ItemData.ItemMaterial;
}

bool UItemBase::GetIsOwned() const
{
	return bIsOwned;
}

void UItemBase::AcquireItem()
{
	this->bIsOwned = true;
}
