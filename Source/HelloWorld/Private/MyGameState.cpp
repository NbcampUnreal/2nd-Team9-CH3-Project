#include "MyGameState.h"
#include "MyPlayerController.h"
#include "MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMyGameState::AMyGameState()
{
	CurrentArtifactCount = 0;
	CurrentLevelName = TEXT("");
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	//레벨 이동시, InputMode를 GameModeOnly로 초기화
	ResetInputMode();
	if (UWorld* World = GetWorld())
	{
		CurrentLevelName = FName(*World->GetName());
	}
	// 블루프린트 노드에 Fade In효과가 끝났을때 StartLevel()이 호출되게함.
	// StartLevel();
	//
	// GetWorldTimerManager().SetTimer(
	// 	HUDUpdateTimerHandle,
	// 	this,
	// 	&AMyGameState::UpdateHUD,
	// 	0.1f,
	// 	true
	// );
}

void AMyGameState::StartLevel()
{
	// if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	// {
	// 	if (AMyPlayerController* SpartaPlayerController = Cast<AMyPlayerController>(PlayerController))
	// 	{
	// 		// 만약 메인 로비라면 HUD를 띄워주지 않음.
	// 		FString CurrentMapName = GetWorld()->GetMapName();
	// 		if (CurrentMapName != LevelMapNames[0])  
	// 		{
	// 			if (!CurrentMapName.Contains("Lobby"))
	// 			{
	// 				UE_LOG(LogTemp, Warning, TEXT("SHOWGAMEHUD!!!"));
	// 				SpartaPlayerController->ShowGameHUD();
	// 				SpartaPlayerController->StartFadeIn();
	// 			}
	// 			else
	// 			{
	// 				UE_LOG(LogTemp, Error, TEXT("SHOWGAMEHUD!!!"));
	// 			}
	// 		}
	// 		
	// 		SpartaPlayerController->SetInputMode(FInputModeGameOnly());
	// 	}
	// }
	//
	// if (UGameInstance* GameInstance = GetGameInstance())
	// {
	// 	if (UMyGameInstance* SpartaGameInstance = Cast<UMyGameInstance>(GameInstance))
	// 	{
	// 		CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
	// 	}
	// }
}

void AMyGameState::EndLevel()
{
	//인스턴스에 값 넘겨주는 구현 필요
}

void AMyGameState::OnGameOver()
{
	// if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	// {
	// 	if (AMyPlayerController* SpartaPlayerController = Cast<AMyPlayerController>(PlayerController))
	// 	{
	// 		SpartaPlayerController->SetPause(true);
	// 		SpartaPlayerController->ShowGameOverMenu();
	// 	}
	// }
}

void AMyGameState::ResetInputMode()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
			{
				MyPlayerController->SetInputMode(FInputModeGameOnly());
				MyPlayerController->bShowMouseCursor = false;
				MyPlayerController->SetPause(false);
			}
		}
	}
}


void AMyGameState::ShowJoinUI()
{
	if (!WBPJoinUI)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		if (!JoinUI)
		{
			JoinUI = CreateWidget<UUserWidget>(World, WBPJoinUI);
			if (JoinUI)
			{
				JoinUI->AddToViewport();
			}
		}
		else if (!JoinUI->IsInViewport())
		{
			JoinUI->AddToViewport();
		}

		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
			{
				MyPlayerController->SetInputMode(FInputModeUIOnly());
				MyPlayerController->bShowMouseCursor = true;
				MyPlayerController->SetPause(true);
			}
		}
	}
}

void AMyGameState::HideJoinUI()
{
	if (JoinUI)
	{
		JoinUI->RemoveFromParent();
		JoinUI = nullptr;
		ResetInputMode();
	}
}

void AMyGameState::SetTargetLevelName(FName NewLevelName)
{
	TargetLevelName = NewLevelName;
}

void AMyGameState::ConfirmMoveLevel()
{
	HideJoinUI();
	EndLevel();
	UGameplayStatics::OpenLevel(this, TargetLevelName);
}

void AMyGameState::DeclineMoveLevel()
{
	HideJoinUI();
	TargetLevelName = TEXT("");
}

void AMyGameState::UpdateHUD()
{
	// if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	// {
	// 	if (AMyPlayerController* SpartaPlayerController = Cast<AMyPlayerController>(PlayerController))
	// 	{
	// 		if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
	// 		{
	// 			if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
	// 			{
	// 				float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
	// 				if (RemainingTime < 0)
	// 				{
	// 					RemainingTime = 0;
	// 				}
	// 				TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
	// 			}
	//
	// 			if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
	// 			{
	// 				if (UGameInstance* GameInstance = GetGameInstance())
	// 				{
	// 					if (UMyGameInstance* SpartaGameInstance = Cast<UMyGameInstance>(GameInstance))
	// 					{
	// 						ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
	// 					}
	// 				}
	// 			}
	//
	// 			if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
	// 			{
	// 				LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
	// 			}
	// 		}
	// 	}
	// }
}