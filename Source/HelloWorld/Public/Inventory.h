#pragma once

#include "CoreMinimal.h"
#include "Inventory.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon      UMETA(DisplayName = "Weapon"),
	Artifact    UMETA(DisplayName = "Artifact"),
	WeaponPart  UMETA(DisplayName = "Weapon Part")
};

USTRUCT()
struct FInventory : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;
	
	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

	// 아이템 썸네일 경로 (Soft Object Path)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UMaterialInterface> ThumbnailPath;
	
	// 적용할 머터리얼 경로 (Soft Object Path)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UMaterialInterface> MaterialPath;

	// 오브젝트 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector Scale;

	// 무기 데미지 (무기일 경우)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Damage;

	// 총알 개수 (무기일 경우)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 BulletCount;

	// 아이템 소유 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsOwned;
	
};
