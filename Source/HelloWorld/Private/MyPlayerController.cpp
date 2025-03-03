#include "MyPlayerController.h"
#include "ScreenEffectComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyHUD.h"
#include "Components/SlateWrapperTypes.h"
#include "Blueprint/UserWidget.h"


AMyPlayerController::AMyPlayerController() 
{
	ScreenEfc = CreateDefaultSubobject<UScreenEffectComponent>(TEXT("ScreenEffect"));
	PauseMenuAction = nullptr;
	InventoryAction = nullptr;
	MissionAction = nullptr;
	IsMissionOpen = false;
}

void AMyPlayerController::ToggleGamePauseMenu()
{
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());

	// 게임 퍼즈메뉴 인스턴스가 존재하지 않으면 열고, 있으면 닫는 방식
	if (!HUD->GetGamePauseMenuWidget())
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
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());

	// 인벤토리 위젯 인스턴스가 존재하지 않으면 열고, 있으면 닫는 방식
	if (!HUD->GetInventoryWidget())
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
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());
	// 만약 HUD 인스턴스가 없다면 바로 아래 if문 실행되어서는 안됨.
	if (HUD->GetHUDWidget())
	{
		ESlateVisibility HUDVisibility = HUD->GetHUDVisibility();

		if (HUDVisibility == ESlateVisibility::Visible)
		{
			IsMissionOpen = !IsMissionOpen;
			if (IsMissionOpen)
			{
				HUD->HideMission();
			}
			else
			{
				HUD->ShowMission();
			}
		}
	}
}
