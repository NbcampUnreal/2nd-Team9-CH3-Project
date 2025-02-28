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
	int32 CurrentArtifactCount;
	
	// 레벨 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	FName CurrentLevelName;  // 현재 레벨의 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;  // 각 레벨을 이름으로 담을 배열
	UPROPERTY()
	FName TargetLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WBPJoinUI;
	UPROPERTY()
	UUserWidget* JoinUI;
	
	// 타이머 핸들러
	FTimerHandle LevelTimerHandle;  // 레벨 타이머
	FTimerHandle HUDUpdateTimerHandle;  // HUD 0.1초마다 업데이트할 타이머

	// 멤버 함수
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
	

	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartLevel();
	void EndLevel();
	void UpdateHUD();
};