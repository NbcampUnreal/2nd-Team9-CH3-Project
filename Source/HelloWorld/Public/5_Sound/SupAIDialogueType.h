#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SupAIDialogueType.generated.h"

UENUM(BlueprintType)
enum class EDialogueSupAI : uint8
{
	Intro1 UMETA(DisplayName = "Intro1"),
	Intro2 UMETA(DisplayName = "Intro2"),
	Intro3 UMETA(DisplayName = "Intro3"),
	Intro4 UMETA(DisplayName = "Intro4"),
	Intro5 UMETA(DisplayName = "Intro5"),
	Intro6 UMETA(DisplayName = "Intro6"),
	BattleTutorial UMETA(DisplayName = "BattleTutorial"),
	MoveAndLook UMETA(DisplayName = "MoveAndLook"),
	Fire UMETA(DisplayName = "Fire"),
	Dash UMETA(DisplayName = "Dash"),
	GrabWall UMETA(DisplayName = "GrabWall"),
	End UMETA(DisplayName = "End")
};

USTRUCT(BlueprintType)
struct HELLOWORLD_API FDialogueDataSupAI
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	USoundBase* DialogueAudio = nullptr;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString SubtitleText;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float MinTimeBetweenPlaybacks = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float AudioDuration = 2.0f;
};

USTRUCT(BlueprintType)
struct FDialogueDataSupAITableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EDialogueSupAI DialogueT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FDialogueDataSupAI DialogueD;
};