#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MyGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AMyPlayerController::AMyPlayerController() 
	: HUDWidgetClass(nullptr),
	  MainMenuWidgetClass(nullptr),
      GameOverMenuWidgetClass(nullptr),
	  FadeInAndOutWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MainMenuWidgetInstance(nullptr),
	  GameOverMenuWidgetInstance(nullptr),
	  FadeInAndOutWidgetInstance(nullptr)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("Menu"))
	{
		ShowMainMenu();
	}
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

void AMyPlayerController::HideMainMenu()
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
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

void AMyPlayerController::StartGame()
{
	if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
	{
		if (!GameState)
		{
			UE_LOG(LogTemp, Error, TEXT("GameState is nullptr!"));
		}
		
		if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			if (!MyGameInstance)
			{
				UE_LOG(LogTemp, Error, TEXT("GameInstance is nullptr!"));
			}
			MyGameInstance->TotalScore = 0;
			UGameplayStatics::OpenLevel(GetWorld(), GameState->LevelMapNames[0]);
			HideMainMenu();
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
		if (UFunction* FadeInFunc = HUDWidgetInstance->FindFunction(FName("FadeInAnimFunction")))
		{
			HUDWidgetInstance->ProcessEvent(FadeInFunc, nullptr);
		}
		
		if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
		{
			GameState->UpdateHUD();
		}
	}
}

void AMyPlayerController::StartFadeOut()
{
	if (HUDWidgetClass)
	{
		if (UFunction* FadeOutFunc = HUDWidgetInstance->FindFunction(FName("FadeOutAnimFunction")))
		{
			HUDWidgetInstance->ProcessEvent(FadeOutFunc, nullptr);
		}
		
		if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
		{
			GameState->UpdateHUD();
		}
	}
}