#include "0_Framework/MyGameMode.h"

#include "0_Framework/MyGameInstance.h"
#include "0_Framework/MyGameState.h"
#include "1_UI/MyPlayerController.h" 
#include "3_Inventory/DevCharacter.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "5_Sound/DialogueSubsystem.h"
#include "5_Sound/SupAIDialogueType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ADevCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass(); 
	GameStateClass = AMyGameState::StaticClass();
	DialogueSubsystem = nullptr;
	LastPlayedDialogueBossAI = EDialogueBossAI::None;
	CurrentDialogueIndex = 0;
	CurrentLevelID = 0;
	CurrentLevelName = "";
	bIsRandom = false;
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		CurrentLevelName = FName(*World->GetName());
		
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		MyGameInstance = Cast<UMyGameInstance>(GameInstance);
	}

	if (MyGameInstance)
	{
		DialogueSubsystem = MyGameInstance->GetSubsystem<UDialogueSubsystem>();
		DialogueSubsystem->LoadDataTables();
		
		if (DialogueSubsystem)
		{
			DialogueSubsystem->OnDialogueFinished.AddDynamic(this, &AMyGameMode::OnDialogueFinished);
		}
	}

	if (CurrentLevelName == TEXT("TutorialLevel"))
	{
		StartTutorial();
	}
	else if (CurrentLevelName == TEXT("MainLobbyLevel"))
	{
		StartMainLobby();
	}
	else if (CurrentLevelName == TEXT("StageLevel1"))
	{
		EnterLevel(2, true);
	}
	else if (CurrentLevelName == TEXT("StageLevel2"))
	{
		EnterLevel(3, true);
	}
	else if (CurrentLevelName == TEXT("BossStageLevel"))
	{
		EnterLevel(4, true);
	}
}

void AMyGameMode::EnterLevel(int32 LevelID, bool bIsRandomMode)
{
	GetWorldTimerManager().ClearTimer(NextBossAIDialogueTimerHandle);
	CurrentLevelID = LevelID;
	CurrentDialogueIndex = 0;
	bIsRandom = bIsRandomMode;
	
	SetupLevelDialogueBossAI(LevelID);
	
	if (LevelDialogue.Num() > 0 && DialogueSubsystem)
	{
		float InitialDelay = FMath::RandRange(2.0f, 5.0f);
		GetWorldTimerManager().SetTimer(
			NextBossAIDialogueTimerHandle,
			this,
			&AMyGameMode::PlayNextLevelDialogueBossAI,
			InitialDelay,
			false
			);
	}
}

void AMyGameMode::ExitLevel()
{
	GetWorldTimerManager().ClearTimer(NextBossAIDialogueTimerHandle);
	LevelDialogue.Empty();

	if (DialogueSubsystem)
	{
		DialogueSubsystem->StopCurrentDialogue();
	}
	if (CurrentLevelName == FName("MainLobbyLevel"))
	{
		if (MyGameInstance)
		{
			MyGameInstance->SetIsMainVisited(true);
		}
	}
}

void AMyGameMode::PlayNextLevelDialogueBossAI()
{
	if (LevelDialogue.Num() == 0 || !DialogueSubsystem)
	{
		return;
	}

	EDialogueBossAI SelectedDialogueBossAI;

	if (bIsRandom)
	{
		int32 RandomIndex;
		if (LevelDialogue.Num() > 1)
		{
			do
			{
				RandomIndex = FMath::RandRange(0, LevelDialogue.Num() - 1);
				SelectedDialogueBossAI = LevelDialogue[RandomIndex];
			}
			while (SelectedDialogueBossAI == LastPlayedDialogueBossAI && LevelDialogue.Num() > 1);
		}
		else
		{
			SelectedDialogueBossAI = LevelDialogue[0];
			RandomIndex = 0;
		}
	
		if (RandomIndex >= 0 && RandomIndex < LevelDialogue.Num())
		{
			LevelDialogue.RemoveAt(RandomIndex);
			UE_LOG(LogTemp, Warning, TEXT("Removed dialogue at index %d. Remaining: %d"), 
				RandomIndex, LevelDialogue.Num());
		}
	}
	else
	{
		if (CurrentDialogueIndex >= LevelDialogue.Num())
		{
			return;
		}

		SelectedDialogueBossAI = LevelDialogue[CurrentDialogueIndex];
		CurrentDialogueIndex++;
	}

	LastPlayedDialogueBossAI = SelectedDialogueBossAI;
	DialogueSubsystem->PlayBossAIDialogue(SelectedDialogueBossAI);
	
}

void AMyGameMode::OnDialogueFinished(EDialogueBossAI DialogueTypeBossAI)
{
	bool bShouldContinue = bIsRandom ? (LevelDialogue.Num() > 0) : (CurrentDialogueIndex < LevelDialogue.Num());

	if (DialogueSubsystem)
	{
		DialogueSubsystem->StopCurrentDialogue();
	}
	
	if (bShouldContinue)
	{
		//랜덤 재생인 경우 대사 간 지연시간 길게, 순차 재생인 경우 지연시간 짧게
		float NextLevelDelay = bIsRandom ? FMath::RandRange(5.0f, 10.0f) : 0.5f;
		GetWorldTimerManager().SetTimer(
			NextBossAIDialogueTimerHandle,
			this,
			&AMyGameMode::PlayNextLevelDialogueBossAI,
			NextLevelDelay,
			false
			);
	}
	else
	{
		if (!bIsRandom)
		{
			if (MyGameInstance->GetIsMainVisited())
			{
				return;
			}
			if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				if (AParagonAssetCharacter* PlayerCharacter = Cast<AParagonAssetCharacter>(PlayerController->GetPawn()))
				{
					if (UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement())
					{
						//PlayerController->SetIgnoreLookInput(false);
						PlayerController->SetIgnoreMoveInput(false);
						MovementComponent->SetMovementMode(MOVE_Walking);
					}
				}
			}
		}
		// if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		// {
		// 	if (APawn* PlayerPawn = MyPlayerController->GetPawn())
		// 	{
		// 		PlayerPawn->EnableInput(MyPlayerController);
		// 	}
		// }
	}
}

void AMyGameMode::OnTutorialDialogueFinished(EDialogueSupAI DialogueTypeSupAI)
{
	if (MyGameInstance)
	{
		UDialogueSubsystem* DialogueSystem = MyGameInstance->GetSubsystem<UDialogueSubsystem>();
		if (DialogueSystem)
		{
			// 바인딩 해제
			DialogueSystem->OnDialogueSupAIFinished.RemoveDynamic(this, &AMyGameMode::OnTutorialDialogueFinished);
		}
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLobbyLevel"));
}


void AMyGameMode::SetupLevelDialogueBossAI(int32 LevelID)
{
	LevelDialogue.Empty();

	switch (LevelID)
	{
	case 1:  //MainLobby
		LevelDialogue.Add(EDialogueBossAI::Intro1);
		LevelDialogue.Add(EDialogueBossAI::Intro2);
		LevelDialogue.Add(EDialogueBossAI::Intro3);
		LevelDialogue.Add(EDialogueBossAI::StayThere);
		break;
	case 2:  //Stage1
		LevelDialogue.Add(EDialogueBossAI::Stage1_1);
		LevelDialogue.Add(EDialogueBossAI::Stage1_2);
		LevelDialogue.Add(EDialogueBossAI::Stage1_3);
		LevelDialogue.Add(EDialogueBossAI::IllKillYouSoon);
		LevelDialogue.Add(EDialogueBossAI::IfYouDontStopIllKillYouSoon);
		break;
	case 3:  //Stage2
		LevelDialogue.Add(EDialogueBossAI::Stage2_1);
		LevelDialogue.Add(EDialogueBossAI::Stage2_2);
		LevelDialogue.Add(EDialogueBossAI::IfYouDontStopIllKillYouSoon);
		break;
	case 4: //BossStage
		LevelDialogue.Add(EDialogueBossAI::BossStage1);
		LevelDialogue.Add(EDialogueBossAI::BossStage2);
		LevelDialogue.Add(EDialogueBossAI::BossStage3);
		LevelDialogue.Add(EDialogueBossAI::BossStage4);
		LevelDialogue.Add(EDialogueBossAI::BossStage5);
		LevelDialogue.Add(EDialogueBossAI::BossStage6);
		LevelDialogue.Add(EDialogueBossAI::BossStage7);
		LevelDialogue.Add(EDialogueBossAI::BossStage9);
		break;
	default:
		break;
	}
}


void AMyGameMode::StartTutorial()
{
	if (MyGameInstance)
	{
		UDialogueSubsystem* DialogueSystem = MyGameInstance->GetSubsystem<UDialogueSubsystem>();
		if (DialogueSystem)
		{
			DialogueSystem->OnDialogueSupAIFinished.AddDynamic(this, &AMyGameMode::OnTutorialDialogueFinished);
			// 전투 튜토리얼 대사 재생
			TArray<EDialogueSupAI> DialogueSequence;
			DialogueSequence.Add(EDialogueSupAI::Intro1);
			DialogueSequence.Add(EDialogueSupAI::Intro2);
			DialogueSequence.Add(EDialogueSupAI::Intro3);
			DialogueSequence.Add(EDialogueSupAI::Intro4);
			DialogueSequence.Add(EDialogueSupAI::Intro5);
			DialogueSequence.Add(EDialogueSupAI::Intro6);
			DialogueSequence.Add(EDialogueSupAI::BattleTutorial);
			DialogueSequence.Add(EDialogueSupAI::MoveAndLook);
			DialogueSequence.Add(EDialogueSupAI::Fire);
			DialogueSequence.Add(EDialogueSupAI::Dash);
			DialogueSequence.Add(EDialogueSupAI::GrabWall);
			DialogueSequence.Add(EDialogueSupAI::End);
			DialogueSystem->PlaySupAIDialogueSequence(DialogueSequence);
		}
	}
}

void AMyGameMode::StartMainLobby()
{
	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (AParagonAssetCharacter* PlayerCharacter = Cast<AParagonAssetCharacter>(PlayerController->GetPawn()))
		{
			PlayerCharacter->SwitchCanSpecialAction();
			if (!MyGameInstance->GetIsMainVisited())
			{
				if (UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement())
				{
					if (!DialogueSubsystem->IsPlayingDialogue())
					{
						//PlayerController->SetIgnoreLookInput(true);
						PlayerController->SetIgnoreMoveInput(true);
						MovementComponent->DisableMovement();
					}
				}
			}
		}
	}
	if (!MyGameInstance->GetIsMainVisited())
	{
		EnterLevel(1, false);
	}
}
