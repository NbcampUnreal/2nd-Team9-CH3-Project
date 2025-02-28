#include "MyGameInstance.h"
#include "InventoryManager.h"

UMyGameInstance::UMyGameInstance(): InventoryManager(nullptr), ItemDataTable(nullptr)
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
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