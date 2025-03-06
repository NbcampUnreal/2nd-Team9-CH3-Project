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

	UPROPERTY()
	TArray<FName> UsedTriggerBox;
	// 데이터 테이블 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	UDataTable* ItemDataTable;
	//총 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;

	bool bIsMainVisited;

	UFUNCTION(BlueprintCallable, Category = "Level")
	void MarkTriggerBoxAsUsed(FName Target);
	UFUNCTION(BlueprintCallable, Category = "Level")
	bool WasTriggerBoxUsed(FName Target) const;
	
	//PowerCoreCount GetSet
	int32 GetPowerCoreCount() const;
	void SetPowerCoreCount(int32 NewPowerCoreCount);
	//bIsMainVisited GetSet
	bool GetIsMainVisited() const;
	void SetIsMainVisited(bool NewIsMainVisited);

private:
	int32 PowerCorePartsCount;
};
