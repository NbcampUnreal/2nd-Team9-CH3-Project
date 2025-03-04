// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "UObject/NoExportTypes.h"
#include "InventoryManager.generated.h"

class UItemBase;

UCLASS()
class HELLOWORLD_API UInventoryManager : public UObject
{
	GENERATED_BODY()

public:
	// 아이템 데이터 테이블 로드
	void InitializeInventoryFromDataTable(const UDataTable* ItemDataTable);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	UItemBase* GetItemFromID(const FName ItemID);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<UWeaponParts*> GetWeaponParts(FName WeaponName);

protected:
	TArray<UItemBase*> Inventory;
};
