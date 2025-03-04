#include "0_Framework/MyGameMode.h"

#include "0_Framework/MyGameInstance.h"
#include "0_Framework/MyGameState.h"
#include "1_UI/MyPlayerController.h" 
#include "3_Inventory/DevCharacter.h"
#include "5_Sound/DialogueSubsystem.h"
#include "5_Sound/SupAIDialogueType.h"
#include "Kismet/GameplayStatics.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ADevCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass(); 
	GameStateClass = AMyGameState::StaticClass();
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
		DialogueSubsystem = GameInstance->GetSubsystem<UDialogueSubsystem>();

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
	// else if (CurrentLevelName == TEXT("StageLevel1"))
	// {
	// 	StartStage1();
	// }
	// else if (CurrentLevelName == TEXT("StageLevel2"))
	// {
	// 	StartStage2();
	// }
	// else if (CurrentLevelName == TEXT("BossStageLevel"))
	// {
	// 	StartBossStage();
	// }
}

void AMyGameMode::EnterLevel(int32 LevelID)
{
	GetWorldTimerManager().ClearTimer(NextBossAIDialogueTimerHandle);
	CurrentLevelID = LevelID;
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
}

void AMyGameMode::PlayNextLevelDialogueBossAI()
{
	if (LevelDialogue.Num() == 0 && !DialogueSubsystem)
	{
		return;
	}

	EDialogueBossAI SelectedDialogueBossAI;
	if (LevelDialogue.Num() > 1)
	{
		do
		{
			int32 RandomIndex = FMath::RandRange(0, LevelDialogue.Num() - 1);
			SelectedDialogueBossAI = LevelDialogue[RandomIndex];
		}
		while (SelectedDialogueBossAI == LastPlayedDialogueBossAI && LevelDialogue.Num() > 1);
	}
	else
	{
		SelectedDialogueBossAI = LevelDialogue[0];
	}

	LastPlayedDialogueBossAI = SelectedDialogueBossAI;
	DialogueSubsystem->PlayBossAIDialogue(SelectedDialogueBossAI);
}

void AMyGameMode::OnDialogueFinished(EDialogueBossAI DialogueTypeBossAI)
{
	if (LevelDialogue.Num() > 0)
	{
		float NextLevelDelay = FMath::RandRange(5.0f, 10.0f);
		GetWorldTimerManager().SetTimer(
			NextBossAIDialogueTimerHandle,
			this,
			&AMyGameMode::PlayNextLevelDialogueBossAI,
			NextLevelDelay,
			false
			);
	}
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
		LevelDialogue.Add(EDialogueBossAI::BossStage8);
		LevelDialogue.Add(EDialogueBossAI::BossStage9);
		break;
	default:
		break;
	}
}


void AMyGameMode::StartTutorial()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance)
	{
		UDialogueSubsystem* DialogueSystem = GameInstance->GetSubsystem<UDialogueSubsystem>();
		if (DialogueSystem)
		{
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
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance)
	{
		UDialogueSubsystem* DialogueSystem = GameInstance->GetSubsystem<UDialogueSubsystem>();
		if (DialogueSystem)
		{
			TArray<EDialogueBossAI> DialogueSequence;
			DialogueSequence.Add(EDialogueBossAI::Intro1);
			DialogueSequence.Add(EDialogueBossAI::Intro2);
			DialogueSequence.Add(EDialogueBossAI::Intro3);
			DialogueSequence.Add(EDialogueBossAI::StayThere);
			DialogueSequence.Add(EDialogueBossAI::IllKillYouSoon);
			DialogueSequence.Add(EDialogueBossAI::Stage1_1);
			DialogueSequence.Add(EDialogueBossAI::Stage1_2);
			DialogueSequence.Add(EDialogueBossAI::Stage1_3);
			DialogueSequence.Add(EDialogueBossAI::IfYouDontStopIllKillYouSoon);
			DialogueSequence.Add(EDialogueBossAI::Stage2_1);
			DialogueSequence.Add(EDialogueBossAI::Stage2_2);
			DialogueSequence.Add(EDialogueBossAI::BossStage1);
			DialogueSequence.Add(EDialogueBossAI::BossStage2);
			DialogueSequence.Add(EDialogueBossAI::BossStage3);
			DialogueSequence.Add(EDialogueBossAI::BossStage4);
			DialogueSequence.Add(EDialogueBossAI::BossStage5);
			DialogueSequence.Add(EDialogueBossAI::BossStage6);
			DialogueSequence.Add(EDialogueBossAI::BossStage7);
			DialogueSequence.Add(EDialogueBossAI::BossStage8);
			DialogueSequence.Add(EDialogueBossAI::BossStage9);
			DialogueSystem->PlayBossAIDialogueSequence(DialogueSequence);
		}
	}
}