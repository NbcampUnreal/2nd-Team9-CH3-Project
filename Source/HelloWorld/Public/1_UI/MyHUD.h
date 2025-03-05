#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

UCLASS()
class HELLOWORLD_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();
	virtual void BeginPlay() override;

	// 멤버 변수 - UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu|Main")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu|GameOver")
	TSubclassOf<UUserWidget> GameOverMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu|GamePause")
	TSubclassOf<UUserWidget> GamePauseMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Log")
	TSubclassOf<UUserWidget> CombatLogWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP_Bar")
	TSubclassOf<UUserWidget> EnemyHPBarWidgetClass;
	
	// 메서드 - UI 관련 
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintPure, Category = "Menu|GamePause")
	UUserWidget* GetGamePauseMenuWidget() const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UUserWidget* GetInventoryWidget() const;

	UUserWidget* GetCombatLogWidget();

	void CreateCombatLogWidget();

	UFUNCTION(BlueprintPure, Category = "HUD")
	ESlateVisibility GetHUDVisibility() const;

	// HUD
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();  // HUD 띄우기
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideGameHUD();  // HUD 숨기기
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateCharacterHPBar();  // 캐릭터 체력 바 업데이트
	
	// 메뉴
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void ShowMainMenu();  // 게임 시작 창 띄우기
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void HideMainMenu();  // 게임 시작 창 숨기기
	
	UFUNCTION(BlueprintCallable, Category = "Menu|GamePause")
	void ShowGamePauseMenu(); // 게임 퍼즈 창 띄우기
	UFUNCTION(BlueprintCallable, Category = "Menu|GamePause")
	void HideGamePauseMenu(); // 게임 퍼즈 창 숨기기
	UFUNCTION(BlueprintCallable, Category = "Menu|GameOver")
	void ShowGameOverMenu(); // 게임 오버 창 띄우기

	// 인벤토리
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowInventory();  // 인벤토리 띄우기
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HideInventory();  // 인벤토리 숨기기

	// 미션창
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void ShowMission();  // 미션 창 띄우기
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void HideMission();  // 미션 창 숨기기
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void UpdateMission();  // 미션 창 업데이트
	
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeIn(float Duration);  // 레벨에 진입 시 서서히 화면이 밝아지는 효과
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeOut(float Duration);  // 레벨이 변경되기 전 서서히 화면이 어두워지는 효과
	
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void StartGame();  // 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void QuitGame();  // 게임 종료

	void PlayAnimCoreMFinished();  // 코어 미션 완료하면 나타나는 애니메이션
	void PlayAnimBossMFinished();  // 보스 처치 미션 완료하면 나타나는 애니메이션
	void PlayAnimTakeDamageEnemy();  // 일반 적이 데미지를 받았을 때 애니메이션 실행

protected:
	// 멤버 변수 - UI 클래스 UPROPERTY 필수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu|Main")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu|GameOver")
	UUserWidget* GameOverMenuWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu|GamePause")
	UUserWidget* GamePauseMenuWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	UUserWidget* InventoryWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CombatLog")
	UUserWidget* CombatLogWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP_Bar")
	UUserWidget* EnemyHPBarWidgetInstance;
	
};
