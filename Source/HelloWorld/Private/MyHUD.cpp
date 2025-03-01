#include "MyHUD.h"
#include "MyGameState.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ScreenEffectComponent.h"

AMyHUD::AMyHUD()
	: HUDWidgetClass(nullptr),
	  MainMenuWidgetClass(nullptr),
	  GameOverMenuWidgetClass(nullptr),
	  GamePauseMenuWidgetClass(nullptr),
	  InventoryWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MainMenuWidgetInstance(nullptr),
	  GameOverMenuWidgetInstance(nullptr),
	  GamePauseMenuWidgetInstance(nullptr),
	  InventoryWidgetInstance(nullptr)
{
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("Menu"))
	{
		ShowMainMenu();
	}
}

// HUD
UUserWidget* AMyHUD::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AMyHUD::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			HUDWidgetInstance = CreateWidget<UUserWidget>(MyPC, HUDWidgetClass);

			if (HUDWidgetInstance)
			{
				HUDWidgetInstance->AddToViewport();
				
				MyPC->bShowMouseCursor = false;
				MyPC->SetInputMode(FInputModeGameOnly());
			}
		}
		
		if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
		{
			GameState->UpdateHUD();
		}
	}
}

// 메인메뉴
void AMyHUD::ShowMainMenu()
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			MainMenuWidgetInstance = CreateWidget<UUserWidget>(MyPC, MainMenuWidgetClass);

			if (MainMenuWidgetInstance)
			{
				MainMenuWidgetInstance->AddToViewport();

				MyPC->bShowMouseCursor = true;
				MyPC->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

void AMyHUD::HideMainMenu()
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}

// 게임 오버 메뉴
void AMyHUD::ShowGameOverMenu()
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	if (GameOverMenuWidgetClass)
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			GameOverMenuWidgetInstance = CreateWidget<UUserWidget>(MyPC, GameOverMenuWidgetClass);

			if (GameOverMenuWidgetInstance)
			{
				GameOverMenuWidgetInstance->AddToViewport();

				MyPC->bShowMouseCursor = false;
				MyPC->SetInputMode(FInputModeGameOnly());
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(MyPC)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), GameInstance->TotalScore)
					));
				}
			}
		}
		
		if (UFunction* PlayAnimFunc = GameOverMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim")))
		{
			GameOverMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
		}
		
		if (UTextBlock* GameOverText = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName("GameOverText")))
		{
			GameOverText->SetText(FText::FromString(FString::Printf(TEXT("Game Over..."))));
		}
	}
}

// 게임 퍼즈 메뉴
void AMyHUD::ShowGamePauseMenu()
{
	if (GamePauseMenuWidgetInstance)
	{
		GamePauseMenuWidgetInstance->RemoveFromParent();
		GamePauseMenuWidgetInstance = nullptr;
	}

	if (GamePauseMenuWidgetClass)
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			GamePauseMenuWidgetInstance = CreateWidget<UUserWidget>(MyPC, GamePauseMenuWidgetClass);

			if (GamePauseMenuWidgetInstance)
			{
				GamePauseMenuWidgetInstance->AddToViewport();

				MyPC->SetPause(true);
				MyPC->bShowMouseCursor = true;
				MyPC->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

void AMyHUD::HideGamePauseMenu()
{
	if (GamePauseMenuWidgetInstance)
	{
		GamePauseMenuWidgetInstance->RemoveFromParent();
		GamePauseMenuWidgetInstance = nullptr;

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetPause(false);
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}

// 인벤토리
void AMyHUD::ShowInventory()
{
	if (InventoryWidgetInstance)
	{
		InventoryWidgetInstance->RemoveFromParent();
		InventoryWidgetInstance = nullptr;
	}

	if (InventoryWidgetClass)
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			InventoryWidgetInstance = CreateWidget<UUserWidget>(MyPC, InventoryWidgetClass);

			if (InventoryWidgetInstance)
			{
				InventoryWidgetInstance->AddToViewport();

				MyPC->SetPause(true);
				MyPC->bShowMouseCursor = true;
				MyPC->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

void AMyHUD::HideInventory()
{
	if (InventoryWidgetInstance)
	{
		InventoryWidgetInstance->RemoveFromParent();
		InventoryWidgetInstance = nullptr;

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetPause(false);
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}

// 미션
void AMyHUD::ShowMission()
{
	if (HUDWidgetInstance)
	{
		if (UUserWidget* MissionUI = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName("WBP_Mission_UI")))
		{
			MissionUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMyHUD::HideMission()
{
	if (HUDWidgetInstance)
	{
		if (UUserWidget* MissionUI = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName("WBP_Mission_UI")))
		{
			MissionUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// 게임 흐름 관련
void AMyHUD::StartGame()
{
	if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
	{
		if (!GameState)
		{
			UE_LOG(LogTemp, Error, TEXT("GameState is nullptr!"));
		}

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(PC)))
			{
				if (!MyGameInstance)
				{
					UE_LOG(LogTemp, Error, TEXT("GameInstance is nullptr!"));
				}
				MyGameInstance->TotalScore = 0;
				UGameplayStatics::OpenLevel(GetWorld(), GameState->LevelMapNames[0]);
				HideMainMenu();
			}
			PC->SetPause(false);
		}
	}
}

void AMyHUD::ReStartStage()
{
	
}

void AMyHUD::QuitGame()
{
	UWorld* World = GetWorld();
	
	if (APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr)
	{
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
	}
}

// 페이드 효과
void AMyHUD::StartFadeIn(float Duration)
{
	// HUD + FADE 효과 실행
	ShowGameHUD();
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (UScreenEffectComponent* ScreenEffect = PC->FindComponentByClass<UScreenEffectComponent>())
		{
			ScreenEffect->StartFadeIn(Duration);
		}
	}
}

void AMyHUD::StartFadeOut(float Duration)
{
	ShowGameHUD();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (UScreenEffectComponent* ScreenEffect = PC->FindComponentByClass<UScreenEffectComponent>())
		{
			ScreenEffect->StartFadeOut(Duration);
		}
	}
}