#include "0_Framework/MyGameInstance.h"
#include "3_Inventory/InventoryManager.h"
#include "Kismet/GameplayStatics.h"

UMyGameInstance::UMyGameInstance(): InventoryManager(nullptr), ItemDataTable(nullptr)
{
	TotalScore = 0;
	PowerCorePartsCount = 0;
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
	PowerCorePartsCount = NewPowerCoreCount;
}

int32 UMyGameInstance::GetPowerCoreCount() const
{
	return PowerCorePartsCount;
}