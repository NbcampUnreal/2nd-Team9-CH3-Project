#include "0_Framework/MyGameInstance.h"
#include "1_UI/MyFunctionLibrary.h"
#include "1_UI/MyHUD.h"
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

	InventoryManager = NewObject<UInventoryManager>(this);

	if (ItemDataTable)
	{
		InventoryManager->InitializeInventoryFromDataTable(ItemDataTable);
	}
}

void UMyGameInstance::MarkTriggerBoxAsUsed(FName Target)
{
	if (!UsedTriggerBox.Contains(Target))
	{
		UsedTriggerBox.Add(Target);
	}
}

bool UMyGameInstance::WasTriggerBoxUsed(FName Target) const
{
	return UsedTriggerBox.Contains(Target);
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
	// ShowEndingCredit();
}

// void UMyGameInstance::EndingCreditFadeOutHandler()
// {
// 	if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))
// 	{
// 		HUD->ShowEndingCredit();
// 		UMyFunctionLibrary::StartFadeIn(this);
//
// 		GetWorld()->GetTimerManager().SetTimer(
// 			EndingCreditFadeInTimer,
// 			this,
// 			&UMyGameInstance::EndingCreditFadeInHandler,
// 			UMyFunctionLibrary::GetFadeDuration(this),
// 			false
// 		);
// 	}
// }
//
// void UMyGameInstance::EndingCreditFadeInHandler()
// {
// 	if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))
// 	{
// 		HUD->PlayEndingCredit();
// 	}
// } 

int32 UMyGameInstance::GetPowerCoreCount() const
{
	return PowerCorePartsCount;
}

// void UMyGameInstance::ShowEndingCredit()
// {
// 	if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))
// 	{
// 		// float FadeDuration = UMyFunctionLibrary::GetFadeDuration(this);
// 		UMyFunctionLibrary::StartFadeOut(this);
//
// 		GetWorld()->GetTimerManager().SetTimer(
// 			EndingCreditFadeOutTimer,
// 			this,
// 			&UMyGameInstance::EndingCreditFadeOutHandler,
// 			UMyFunctionLibrary::GetFadeDuration(this),
// 			false
// 		);
// 	}
// 	// 보스 잡고 -> 페이드아웃 -> 엔딩크레딧 처음 부분 나오고 -> 페이드인 -> 엔딩크레딧 애니메이션 시작
// }
