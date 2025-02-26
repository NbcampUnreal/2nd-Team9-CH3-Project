#include "MyGameState.h"
#include "MyPlayerController.h"
#include "MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

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
		if (AMyPlayerController* SpartaPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			// 만약 메인 로비라면 HUD를 띄워주지 않음.
			FString CurrentMapName = GetWorld()->GetMapName();
			if (CurrentMapName != LevelMapNames[0])  
			{
				if (!CurrentMapName.Contains("Lobby"))
				{
					UE_LOG(LogTemp, Warning, TEXT("SHOWGAMEHUD!!!"));
					SpartaPlayerController->ShowGameHUD();
					SpartaPlayerController->StartFadeIn();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("SHOWGAMEHUD!!!"));
				}
			}
			
			SpartaPlayerController->SetInputMode(FInputModeGameOnly());
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
			if (AMyPlayerController* SpartaPlayerController = Cast<AMyPlayerController>(PlayerController))
			{
				SpartaPlayerController->StartFadeOut();
				SpartaPlayerController->SetInputMode(FInputModeUIOnly());
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
		if (AMyPlayerController* SpartaPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowGameOverMenu();
		}
	}
}

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* SpartaPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					if (RemainingTime < 0)
					{
						RemainingTime = 0;
					}
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (UMyGameInstance* SpartaGameInstance = Cast<UMyGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}