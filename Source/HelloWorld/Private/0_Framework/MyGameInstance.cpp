#include "0_Framework/MyGameInstance.h"
#include "3_Inventory/InventoryManager.h"
#include "Kismet/GameplayStatics.h"

UMyGameInstance::UMyGameInstance(): InventoryManager(nullptr), ItemDataTable(nullptr)
{
	TotalScore = 0;
	PowerCorePartsCount = 0;
	bIsMainVisited = false;
	bIsBossDead = false;
}

void UMyGameInstance::Init()
{
	Super::Init();
	
	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::Init"));
	InventoryManager = NewObject<UInventoryManager>(this);

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

bool UMyGameInstance::GetIsMainVisited() const
{
	return bIsMainVisited;
}

void UMyGameInstance::SetIsMainVisited(bool NewIsMainVisited)
{
	bIsMainVisited = NewIsMainVisited;
}

bool UMyGameInstance::GetIsBossDead() const
{
	return bIsBossDead;
}

void UMyGameInstance::SetIsBossDead(bool bIsDead)
{
	bIsBossDead = bIsDead;
}

int32 UMyGameInstance::GetPowerCoreCount() const
{
	return PowerCorePartsCount;
}
