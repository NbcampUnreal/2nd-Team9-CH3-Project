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
	virtual void BeginPlay() override;

	// 멤버 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 TotalScore;
	int32 CurrentArtifactCount;
	int32 CurrentCharacterHP;

	// 레벨 관련
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;  // 레벨이 존재하는 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;  // 현재 레벨의 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;  // 최대 레벨
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;  // 각 레벨을 이름으로 담을 배열

	// 타이머 핸들러
	FTimerHandle LevelTimerHandle;  // 레벨 타이머
	FTimerHandle HUDUpdateTimerHandle;  // HUD 0.1초마다 업데이트할 타이머

	// 메서드
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartLevel();
	void EndLevel();
	void UpdateHUD();
};