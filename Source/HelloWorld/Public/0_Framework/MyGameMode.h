#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

enum class EDialogueSupAI : uint8;
enum class EDialogueBossAI : uint8;
class UMyGameInstance;
class UDialogueSubsystem;


UCLASS()
class HELLOWORLD_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMyGameMode();

	virtual void BeginPlay() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueFinished, EDialogueBossAI, DialogueType, bool, bIsRandom);

	
	UFUNCTION(BlueprintCallable, Category = "MyGameMode|Dialogue")
	void EnterLevel(int32 LevelID, bool bIsRandomMode);
	UFUNCTION(BlueprintCallable, Category = "MyGameMode|Dialogue")
	void ExitLevel();
	UFUNCTION()
	void AllEnemiesKilled();

	UFUNCTION(BlueprintCallable)
	void OnTutorialDialogueFinished(EDialogueSupAI DialogueTypeSupAI);

private:
	FName CurrentLevelName;
	TArray<EDialogueBossAI> LevelDialogue;
	int32 CurrentLevelID;
	FTimerHandle NextBossAIDialogueTimerHandle;
	UDialogueSubsystem* DialogueSubsystem;
	EDialogueBossAI LastPlayedDialogueBossAI;
	int32 CurrentDialogueIndex;
	bool bIsRandom;
	bool bIsMainLobby;
	UMyGameInstance* MyGameInstance;

	UFUNCTION()
	void OnDialogueFinished(EDialogueBossAI DialogueTypeBossAI);

	void OpenMainLobbyLevel(); // 영민 추가
	
	void PlayNextLevelDialogueBossAI();
	void SetupLevelDialogueBossAI(int32 LevelID);
	
	void StartTutorial();
	void StartMainLobby();
	void StartStage1();
};