#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

class ASpawnEnemyActor;
class UScrollBox;
class UTextBlock;

UCLASS()
class HELLOWORLD_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMyGameState();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesKilled);
	
	// 레벨 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	FName CurrentLevelName;  // 현재 레벨의 이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	FString UILevelName;  // UI에서 보여질 레벨의 이름(EX : [사막 연구소])
	UPROPERTY()
	FName TargetLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WBPJoinUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WBPJoinUI_Boss;
	UPROPERTY()
	UUserWidget* JoinUI;
	//적 관련
	UPROPERTY()
	FOnAllEnemiesKilled OnAllEnemiesKilled;
	
	//// 멤버 변수
	int32 CurrentArtifactCount;
	// 타이머 핸들러
	FTimerHandle LevelTimerHandle;  // 레벨 타이머
	FTimerHandle HUDUpdateTimerHandle;  // HUD 0.1초마다 업데이트할 타이머
	FTimerHandle FadeOutTimer;

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

	void OpenTargetLevel(); // 레벨 이동시 그 레벨 호출 함수

	UFUNCTION(BlueprintCallable, Category = "Level")
	void DeclineMoveLevel();
	//=========WBPJoinUI==========
	
	//레벨 시작될 때 호출
	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void EndLevel();

	// GameState에서 업데이트 되는 정보 관련 함수
	void UpdateHUD();
	void SetMainGunImage(UTexture2D* NewTexture);  // 메인 건 이미지 바뀌는 함수
	void AddKillCount();

	FName GetCurrentLevelName() const; // HUD에서 현재레벨 이름을 가져오기 위해 사용
	int32 GetPowerCorePartsCount() const;
	int32 GetKillCount() const;
	void SetUsedTriggerBox(FName Target);

	// 로그 관련
	void AddCombatLogMessage(const FString& NewMessage);
	void RemoveOldCombatLogWidget();

	UPROPERTY()
	TArray<UUserWidget*> CombatLogWidgetArray; // 추가된 CombatLogWidgets 저장

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponImage")
	UTexture2D* WeaponImage1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponImage")
	UTexture2D* WeaponImage2;

private:
	virtual void BeginPlay() override;

	int32 TotalSpawnedEnemyCount;	//스폰된 적 수
	int32 KillCount;  // 처치한 적 수
	int32 CurrentStage; // 현재 일반 스테이지 번호
	int32 PowerCorePartsCount; // 동력 코어 부품 수 카운트
	int32 MaxPowerCoreParts;
	FName UsedTriggerBox;
	TArray<ASpawnEnemyActor*> EnemySpawners;

	void UpdateDataFromInstance();
	//스테이지별 적 생성
	void SpawnEnemiesInLevel();

	UScrollBox* CombatLogScrollBox; // 전투 로그 ScrollBox 저장
};