#include "MyGameState.h"
#include "MyPlayerController.h"
#include "MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "ScreenEffectComponent.h"
#include "MyHUD.h"

AMyGameState::AMyGameState()
{
	TotalScore = 0;
	CurrentArtifactCount = 0;
	CurrentCharacterHP = 100;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 2;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	
	// 블루프린트 노드에 Fade In효과가 끝났을때 StartLevel()이 호출되게함.
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMyGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 AMyGameState::GetScore() const
{
	return TotalScore;
}

void AMyGameState::AddScore(int32 Amount)
{
	// 나중에 스코어 관련 로직 추가할 때 주석 해제!
	// if (UGameInstance* GameInstance = GetGameInstance())
	// {
	// 	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
	// 	if (MyGameInstance)
	// 	{
	// 		MyGameInstance->AddToScore(Amount);
	// 	}
	// }
}

void AMyGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			// 만약 메인 로비라면 HUD를 띄워주지 않음.
			FString CurrentMapName = GetWorld()->GetMapName();
			if (!CurrentMapName.Contains("Lobby"))  
			{
				// GetHUD()는 반환타입이 AHUD*라 직접 만든 AMyHUD* 타입으로 캐스팅해줘야 함.
				if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))  
				{
					MyHUD->ShowGameHUD();  // HUD에서 호출
					UE_LOG(LogTemp, Warning, TEXT("ShowGameHUD!!!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Screen Effect3!!!"));
				if (UScreenEffectComponent* ScreenEffect = MyPlayerController->ScreenEfc)
				{
					UE_LOG(LogTemp, Warning, TEXT("Screen Effect2!!!"));
					ScreenEffect->StartFadeIn(5.0f);  // ScreenEffectComponent에서 호출
					UE_LOG(LogTemp, Warning, TEXT("Screen Effect1!!!"));
				}
			}
			MyPlayerController->SetInputMode(FInputModeGameOnly());
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UMyGameInstance* SpartaGameInstance = Cast<UMyGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}
}

void AMyGameState::EndLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UMyGameInstance* SpartaGameInstance = Cast<UMyGameInstance>(GameInstance))
		{
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();	
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
			{
				if (UScreenEffectComponent* ScreenEffect = MyPlayerController->ScreenEfc)
				{
					ScreenEffect->StartFadeOut(3.0f);
				}
				MyPlayerController->SetInputMode(FInputModeUIOnly());
			}
		}
	}
	else
	{
		OnGameOver();
	}
}

void AMyGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);  // 현재 레벨을 멈춘뒤..
			if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))  
			{
				MyHUD->ShowGameOverMenu();  // 게임 오버 메뉴 띄우기!
			}
		}
	}
}

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))  
			{
				if (UUserWidget* HUDWidget = MyHUD->GetHUDWidget())  
				{
					if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
					{
						LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
					}
				}
			}
		}
	}
}