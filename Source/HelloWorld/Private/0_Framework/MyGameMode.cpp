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