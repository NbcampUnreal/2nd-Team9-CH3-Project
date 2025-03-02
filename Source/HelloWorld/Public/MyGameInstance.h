#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class UInventoryManager;

UCLASS()
class HELLOWORLD_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UInventoryManager* InventoryManager;

public:
	UMyGameInstance();

	virtual void Init() override;

	UInventoryManager* GetInventoryManager() const;
	
	// 데이터 테이블 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	UDataTable* ItemDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;  // 총 점수
};
