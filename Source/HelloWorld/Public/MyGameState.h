#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

UCLASS()
class HELLOWORLD_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMyGameState();
	int32 TotalScore;
	int32 CurrentArtifactCount;
	int32 CurrentCharacterHP;


	// 영민 추가 코드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	void UpdateHUD();
};