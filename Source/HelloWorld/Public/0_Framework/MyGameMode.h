#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

class UDialogueSubsystem;
enum class EDialogueBossAI : uint8;

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

private:
	FName CurrentLevelName;
	TArray<EDialogueBossAI> LevelDialogue;
	int32 CurrentLevelID;
	FTimerHandle NextBossAIDialogueTimerHandle;
	UDialogueSubsystem* DialogueSubsystem;
	EDialogueBossAI LastPlayedDialogueBossAI;
	int32 CurrentDialogueIndex;
	bool bIsRandom;

	UFUNCTION()
	void OnDialogueFinished(EDialogueBossAI DialogueTypeBossAI);
	UFUNCTION()
	void OnTutorialDialogueFinished(EDialogueSupAI DialogueTypeSupAI);
	
	void PlayNextLevelDialogueBossAI();
	void SetupLevelDialogueBossAI(int32 LevelID);

	void StartTutorial();
	void StartMainLobby();
};
