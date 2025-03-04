// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon      UMETA(DisplayName = "Weapon"),
	Artifact    UMETA(DisplayName = "Artifact"),
	Parts  UMETA(DisplayName = "Weapon Part")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Riffle UMETA(DisplayName = "Riffle"),
	Charging UMETA(DisplayName = "Charging"),
	Melee UMETA(DisplayName = "Melee")
};

UENUM(BlueprintType)
enum class EPartsEffect : uint8
{
	SpeedUP UMETA(DisplayName = "SpeedUP"),
	DamageUp UMETA(DisplayName = "DamageUp")
};

USTRUCT()
struct FItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 아이템 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;
	
	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;
	
	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

	// 아이템 썸네일 2D 텍스쳐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemThumbnail;
	
	// 아이템 머터리얼(무기 전용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UMaterial> ItemMaterial;

	// 무기 데미지(무기 전용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Damage;

	// 무기 종류(무기 전용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponType WeaponType;

	// 무기 파츠 효과(파츠 전용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EPartsEffect PartsEffect;
};
