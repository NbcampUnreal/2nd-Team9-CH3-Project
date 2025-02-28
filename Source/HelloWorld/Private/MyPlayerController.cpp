#include "MyPlayerController.h"

#include "MyGameInstance.h"
#include "MyGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

AMyPlayerController::AMyPlayerController() 
	: HUDWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MainMenuWidgetClass(nullptr),
	  MainMenuWidgetInstance(nullptr),
	  GameOverMenuWidgetClass(nullptr),
	  GameOverMenuWidgetInstance(nullptr),
	  FadeInAndOutWidgetClass(nullptr),
	  FadeInAndOutWidgetInstance(nullptr)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ShowMainMenu();
}

UUserWidget* AMyPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AMyPlayerController::ShowMainMenu()
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

	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	if (FadeInAndOutWidgetInstance)
	{
		FadeInAndOutWidgetInstance->RemoveFromParent();
		FadeInAndOutWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMyPlayerController::ShowGameOverMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("gameOver gogo"));
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

	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	if (FadeInAndOutWidgetInstance)
	{
		FadeInAndOutWidgetInstance->RemoveFromParent();
		FadeInAndOutWidgetInstance = nullptr;
	}

	if (GameOverMenuWidgetClass)
	{
		GameOverMenuWidgetInstance = CreateWidget<UUserWidget>(this, GameOverMenuWidgetClass);
		if (GameOverMenuWidgetInstance)
		{
			GameOverMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		UFunction* PlayAnimFunc = GameOverMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
		if (PlayAnimFunc)
		{
			GameOverMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
		}

		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
		{
			if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TotalScoreText->SetText(FText::FromString(
					FString::Printf(TEXT("Total Score: %d"), GameInstance->TotalScore)
				));
			}
		}

		AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
		if (GameState)
		{
			if (UTextBlock* GameOverText = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName("GameOverText")))
			{
				GameOverText->SetText(FText::FromString(FString::Printf(TEXT("Game Over..."))));
			}
		}
	}
}

void AMyPlayerController::ShowGameHUD()
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
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
		
		if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
		{
			GameState->UpdateHUD();
		}
	}
}

void AMyPlayerController::StartGame(int32 LevelIndex)
{
	if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
	{
		if (UMyGameInstance* SpartaGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			SpartaGameInstance->TotalScore = 0;
			SpartaGameInstance->CurrentLevelIndex = LevelIndex;
			UGameplayStatics::OpenLevel(GetWorld(), GameState->LevelMapNames[LevelIndex]);
		}
	}

	SetPause(false);
}

void AMyPlayerController::ReStartStage()
{
	
}

void AMyPlayerController::QuitGame()
{
	UWorld* World = GetWorld();
	
	if (APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr)
	{
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
	}
}

void AMyPlayerController::StartFadeIn()
{
	if (HUDWidgetClass)
	{
		UFunction* FadeInFunc = HUDWidgetInstance->FindFunction(FName("FadeInAnimFunction"));
		if (FadeInFunc)
		{
			HUDWidgetInstance->ProcessEvent(FadeInFunc, nullptr);
		}

		AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
		if (GameState)
		{
			GameState->UpdateHUD();
		}
	}
}

void AMyPlayerController::StartFadeOut()
{
	if (HUDWidgetClass)
	{
		UFunction* FadeOutFunc = HUDWidgetInstance->FindFunction(FName("FadeOutAnimFunction"));
		if (FadeOutFunc)
		{
			HUDWidgetInstance->ProcessEvent(FadeOutFunc, nullptr);
		}

		AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
		if (GameState)
		{
			GameState->UpdateHUD();
		}
	}
}