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

	UFUNCTION(BlueprintCallable, Category = "MyGameMode")
	void StartTutorial();

	UFUNCTION(BlueprintCallable, Category = "MyGameMode|Dialogue")
	void EnterLevel(int32 LevelID);
	UFUNCTION(BlueprintCallable, Category = "MyGameMode|Dialogue")
	void ExitLevel();

private:
	FName CurrentLevelName;
	TArray<EDialogueBossAI> LevelDialogue;
	int32 CurrentLevelID;
	FTimerHandle NextBossAIDialogueTimerHandle;
	UDialogueSubsystem* DialogueSubsystem;
	EDialogueBossAI LastPlayedDialogueBossAI;

	void PlayNextLevelDialogueBossAI();
	void SetupLevelDialogueBossAI(int32 LevelID);

	UFUNCTION()
	void OnDialogueFinished(EDialogueBossAI DialogueTypeBossAI);
};
