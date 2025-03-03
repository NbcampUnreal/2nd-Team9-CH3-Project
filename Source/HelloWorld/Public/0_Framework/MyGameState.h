#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

class ASpawnEnemyActor;

UCLASS()
class HELLOWORLD_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMyGameState();

	// 레벨 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	FName CurrentLevelName;  // 현재 레벨의 이름
	UPROPERTY()
	FName TargetLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WBPJoinUI;
	// [영민님 WBPJoinUI_Boss 완성되면 주석 해제] - 규혁
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	// TSubclassOf<UUserWidget> WBPJoinUI_Boss;
	UPROPERTY()
	UUserWidget* JoinUI;
	
	//// 멤버 변수
	int32 CurrentArtifactCount;
	// 타이머 핸들러
	FTimerHandle LevelTimerHandle;  // 레벨 타이머
	FTimerHandle HUDUpdateTimerHandle;  // HUD 0.1초마다 업데이트할 타이머

	//// 멤버 함수
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	UFUNCTION(BlueprintCallable, Category = "Controller")
	void ResetInputMode();
	//레벨 관련
	//=========WBPJoinUI==========
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowJoinUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideJoinUI();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void SetTargetLevelName(FName NewLevelName);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void ConfirmMoveLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void DeclineMoveLevel();
	//=========WBPJoinUI==========
	//스테이지별 적 생성
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SpawnEnemiesFromAllSpawners();
	//레벨 시작될 때 호출
	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartLevel();

	//업데이트 HUD
	void UpdateHUD();
	//현재 레벨의 Name Get
	FName GetCurrentLevelName() const; // HUD에서 현재레벨 이름을 가져오기 위해 사용

private:
	virtual void BeginPlay() override;

	int32 TotalSpawnedEnemyCount;	//스폰된 적 수
	
	TArray<ASpawnEnemyActor*> EnemySpawners;
	
	
};