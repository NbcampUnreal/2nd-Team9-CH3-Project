#include "MyHUD.h"
#include "MyGameState.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ParagonAssetCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ScreenEffectComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

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
	
	if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
	{
		FName CurrentLevelName = GameState->GetCurrentLevelName();

		if (CurrentLevelName.ToString().Contains(TEXT("Menu")))
		{
			ShowMainMenu();
		}
	}
}

// HUD
UUserWidget* AMyHUD::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

UUserWidget* AMyHUD::GetGamePauseMenuWidget() const
{
	return GamePauseMenuWidgetInstance;
}

UUserWidget* AMyHUD::GetInventoryWidget() const
{
	return InventoryWidgetInstance;
}

ESlateVisibility AMyHUD::GetHUDVisibility() const
{
	return HUDWidgetInstance->Visibility;
}

void AMyHUD::ShowGameHUD()
{
	// 중복방지용 삭제
	if (InventoryWidgetInstance)
	{
		InventoryWidgetInstance->RemoveFromParent();
		InventoryWidgetInstance = nullptr;
	}

	// 중복방지용 삭제
	if (GamePauseMenuWidgetInstance)
	{
		GamePauseMenuWidgetInstance->RemoveFromParent();
		GamePauseMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			if (!HUDWidgetInstance)
			{
				HUDWidgetInstance = CreateWidget<UUserWidget>(MyPC, HUDWidgetClass);
				HUDWidgetInstance->AddToViewport();
			}

			HUDWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			MyPC->SetPause(false);
			MyPC->bShowMouseCursor = false;
			MyPC->SetInputMode(FInputModeGameOnly());
		}
		
		if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
		{
			GameState->UpdateHUD();
		}
	}
}

void AMyHUD::HideGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

// HUD 관련
void AMyHUD::UpdateCharacterHPBar()
{
	if (!HUDWidgetInstance) return;  // 시작될 때 메인 로비에서는 HUD가 생성되지 않기 때문에 오류 발생해서 넣은 코드

	if (UUserWidget* HPBarWidgetInstance = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("WBP_Character_HP_Bar"))))
	{
		if (UProgressBar* HPBar = Cast<UProgressBar>(HPBarWidgetInstance->GetWidgetFromName(TEXT("Character_HP_Bar"))))
		{
			AParagonAssetCharacter* ParagonCharacter = Cast<AParagonAssetCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			int32 MaxHealth = ParagonCharacter->GetMaxHealth();
			int32 CurrentHealth = ParagonCharacter->GetCurrentHealth();
			float FMaxHealth = MaxHealth;
			float FCurrentHealth = CurrentHealth;
			// HPPercent = 0.0 ~ 1.0 범위의 값이 나오도록 설정
			const float HPPercent = (MaxHealth > 0.f) ? FCurrentHealth / FMaxHealth : 0.f;

			// 프로그레스 바의 퍼센트 설정
			HPBar->SetPercent(HPPercent);

			if (ParagonCharacter->GetHealthState() == EHealthState::Healthy)
			{
				HPBar->SetFillColorAndOpacity(FLinearColor::Green);
			}
			else if (ParagonCharacter->GetHealthState() == EHealthState::Danger)
			{
				HPBar->SetFillColorAndOpacity(FLinearColor::Red);
			}
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

				MyPC->bShowMouseCursor = true;
				MyPC->SetInputMode(FInputModeUIOnly());
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
	HideGameHUD();

	// 중복방지용 삭제
	if (InventoryWidgetInstance)
	{
		InventoryWidgetInstance->RemoveFromParent();
		InventoryWidgetInstance = nullptr;
	}

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
	if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
	{
		FName CurrentLevelName = GameState->GetCurrentLevelName();

		if (!CurrentLevelName.ToString().Contains(TEXT("Lobby")))
		{
			ShowGameHUD();
		}
	}

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
	HideGameHUD();

	// GamePause 중복 방지용 삭제
	if (GamePauseMenuWidgetInstance)
	{
		GamePauseMenuWidgetInstance->RemoveFromParent();
		GamePauseMenuWidgetInstance = nullptr;
	}

	// 혹시 모를 인벤토리 nulltptr초기화
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

	if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
	{
		FName CurrentLevelName = GameState->GetCurrentLevelName();

		if (!CurrentLevelName.ToString().Contains(TEXT("Lobby")))
		{
			ShowGameHUD();
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

void AMyHUD::UpdateMission()
{
	if (HUDWidgetInstance)
	{
		if (UUserWidget* MissionWidgetInstance = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName("WBP_Mission_UI")))
		{
			if (UTextBlock* MissionText = Cast<UTextBlock>(MissionWidgetInstance->GetWidgetFromName(TEXT("GetCoreMission"))))
			{
				if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
				{
					MissionText->SetText(FText::FromString(FString::Printf(TEXT("-   동력코어 획득 [%d / 2]"), GameState->GetPowerCorePartsCount())));
				}
			}
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
				FName MainLobbyLevel = TEXT("MainLobbyLevel");
				UGameplayStatics::OpenLevel(GetWorld(), MainLobbyLevel);
				HideMainMenu();
			}
			PC->SetPause(false);
		}
	}
}

void AMyHUD::QuitGame()
{
	UWorld* World = GetWorld();
	
	if (APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr)
	{
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
	}
}

void AMyHUD::PlayAnimCoreMFinished()
{
	if (HUDWidgetInstance)
	{
		UUserWidget* MissionWidgetInstance = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("WBP_Mission_UI")));
		if (UFunction* CoreAnimFunc = MissionWidgetInstance->FindFunction(FName("CoreMFinishedFunction")))
		{
			MissionWidgetInstance->ProcessEvent(CoreAnimFunc, nullptr);
		}
	}
}

void AMyHUD::PlayAnimBossMFinished()
{
	if (HUDWidgetInstance)
	{
		UUserWidget* MissionWidgetInstance = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("WBP_Mission_UI")));
		if (UFunction* BossAnimFunc = MissionWidgetInstance->FindFunction(FName("BossMFinishedFunction")))
		{
			MissionWidgetInstance->ProcessEvent(BossAnimFunc, nullptr);
		}
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


