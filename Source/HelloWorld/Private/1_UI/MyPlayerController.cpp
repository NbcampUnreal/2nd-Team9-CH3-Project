#include "1_UI/MyPlayerController.h"
#include "1_UI/ScreenEffectComponent.h"
#include "EnhancedInputSubsystems.h"
#include "1_UI/MyHUD.h"
#include "Components/SlateWrapperTypes.h"


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
	//IsGamePauseMenuOpen = !IsGamePauseMenuOpen;
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
	//IsInventoryOpen = !IsInventoryOpen;
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
	IsMissionOpen = !IsMissionOpen;
	AMyHUD* HUD = Cast<AMyHUD>(GetHUD());

	if (HUD->GetHUDVisibility() == ESlateVisibility::Visible)
	{
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

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		return;
	}
	
	if (!DefaultMappingContext)
	{
		return;
	}
	
	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}
