#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BossAIDialogueType.generated.h"

//보스AI
UENUM(BlueprintType)
enum class EDialogueBossAI : uint8
{
	Intro1 UMETA(DisplayName = "Intro1"),
	Intro2 UMETA(DisplayName = "Intro2"),
	Intro3 UMETA(DisplayName = "Intro3"),
	StayThere UMETA(DisplayName = "StayThere"),
	IllKillYouSoon UMETA(DisplayName = "IllKillYouSoon"),
	Stage1_1 UMETA(DisplayName = "Stage1_1"),
	Stage1_2 UMETA(DisplayName = "Stage1_2"),
	Stage1_3 UMETA(DisplayName = "Stage1_3"),
	IfYouDontStopIllKillYouSoon UMETA(DisplayName = "IfYouDontStopIllKillYouSoon"),
	Stage2_1 UMETA(DisplayName = "Stage2_1"),
	Stage2_2 UMETA(DisplayName = "Stage2_2"),
	BossStage1 UMETA(DisplayName = "BossStage1"),
	BossStage2 UMETA(DisplayName = "BossStage2"),
	BossStage3 UMETA(DisplayName = "BossStage3"),
	BossStage4 UMETA(DisplayName = "BossStage4"),
	BossStage5 UMETA(DisplayName = "BossStage5"),
	BossStage6 UMETA(DisplayName = "BossStage6"),
	BossStage7 UMETA(DisplayName = "BossStage7"),
	BossStage8 UMETA(DisplayName = "BossStage8"),
	BossStage9 UMETA(DisplayName = "BossStage9"),
	ExecuteSuit UMETA(DisplayName = "ExecuteSuit"),
	IgnoreThat UMETA(DisplayName = "IgnoreThat"),
	FindCore UMETA(DisplayName = "FindCore"),
	SuitIsReady UMETA(DisplayName = "SuitIsReady"),
	None UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FDialogueDataBossAI
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	USoundBase* DialogueAudio = nullptr;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString SubtitleText;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float MinTimeBetweenPlaybacks = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float AudioDuration = 3.0f;
};

USTRUCT(BlueprintType)
struct FDialogueDataBossAITableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EDialogueBossAI DialogueT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FDialogueDataBossAI DialogueD;
};