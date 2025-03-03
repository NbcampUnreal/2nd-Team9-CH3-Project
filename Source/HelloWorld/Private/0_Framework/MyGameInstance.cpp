#include "0_Framework/MyGameInstance.h"
#include "3_Inventory/InventoryManager.h"

UMyGameInstance::UMyGameInstance(): InventoryManager(nullptr), ItemDataTable(nullptr)
{
	TotalScore = 0;
	PowerCoreCount = 0;
}

void UMyGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::Init"));
	InventoryManager = NewObject<UInventoryManager>();

	if (ItemDataTable)
	{
		InventoryManager->InitializeInventoryFromDataTable(ItemDataTable);
	}
}

UInventoryManager* UMyGameInstance::GetInventoryManager() const
{
	return InventoryManager;
}

void UMyGameInstance::SetPowerCoreCount(int NewPowerCoreCount)
{
	PowerCoreCount = NewPowerCoreCount;
}

int32 UMyGameInstance::GetPowerCoreCount() const
{
	return PowerCoreCount;
}