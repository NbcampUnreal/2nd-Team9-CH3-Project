// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "UObject/Object.h"
#include "ItemBase.generated.h"

UCLASS()
class HELLOWORLD_API UItemBase : public UObject
{
	GENERATED_BODY()
protected:
	// 아이템 구조체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FItem ItemData;

	// 플레이어가 해당 아이템을 가지고 있는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsOwned = false;

public:
	UItemBase();

	UFUNCTION()
	virtual void InitializeItem(const FItem InputData)
	{
		ItemData = InputData;
		// 기본 무기 활성화
		if (ItemData.ItemID.Compare("Weapon_1") == 0)
		{
			bIsOwned = true;
		}
	}
	
	UFUNCTION(BlueprintCallable, Category = "Item")
	FName GetItemID() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FName GetItemName() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	EItemType GetItemType() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	TSoftObjectPtr<UTexture2D> GetItemThumbnail() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	TSoftObjectPtr<UMaterial> GetItemMaterial() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool GetIsOwned() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void AcquireItem();
};
