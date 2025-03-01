#include "MyPlayerController.h"
#include "ScreenEffectComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyHUD.h"


AMyPlayerController::AMyPlayerController() 
{
	ScreenEfc = CreateDefaultSubobject<UScreenEffectComponent>(TEXT("ScreenEffect"));
	PauseMenuAction = nullptr;
	InventoryAction = nullptr;
	MissionAction = nullptr;
	IsGamePauseMenuOpen = false;
	IsInventoryOpen = false;
	IsMissionOpen = false;
}

void AMyPlayerController::ToggleGamePauseMenu()
{
	IsGamePauseMenuOpen = !IsGamePauseMenuOpen;
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());

	if (IsGamePauseMenuOpen)
	{
		HUD->ShowGamePauseMenu();
	}
	else
	{
		HUD->HideGamePauseMenu();
	}
}

void AMyPlayerController::ToggleInventory()
{
	IsInventoryOpen = !IsInventoryOpen;
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());

	if (IsInventoryOpen)
	{
		HUD->ShowInventory();
	}
	else
	{
		HUD->HideInventory();
	}
}

void AMyPlayerController::ToggleMission()
{
	IsMissionOpen = !IsMissionOpen;
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());

	if (IsMissionOpen)
	{
		HUD->ShowMission();
	}
	else
	{
		HUD->HideMission();
	}
}
