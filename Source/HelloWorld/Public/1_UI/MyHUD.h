#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UImage;
class UTexture2D;

UCLASS()
class HELLOWORLD_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartFadeOut);

	virtual void BeginPlay() override;


	UPROPERTY()
	FStartFadeOut OnStartFadeOut;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|PowerCore")
	TSubclassOf<UUserWidget> ItemPowerCoreWidgetClass1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|PowerCore")
	TSubclassOf<UUserWidget> ItemPowerCoreWidgetClass2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NoPowerOnSuit")
	TSubclassOf<UUserWidget> NoPowerOnSuitWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EndingCredit")
	TSubclassOf<UUserWidget> EndingCreditWidgetClass;
	
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

	void FadeOutPlayerDead(); // 죽으면 게임 오버 메뉴..

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateBossHPBar();  // 보스 체력 바 업데이트

	// 엔딩 크레딧
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowEndingCredit();

	// 메뉴
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void ShowMainMenu();  // 게임 시작 창 띄우기
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void HideMainMenu();  // 게임 시작 창 숨기기
	
	UFUNCTION(BlueprintCallable, Category = "Menu|GamePause")
	void ShowGamePauseMenu();  // 게임 퍼즈 창 띄우기
	UFUNCTION(BlueprintCallable, Category = "Menu|GamePause")
	void HideGamePauseMenu();  // 게임 퍼즈 창 숨기기
	UFUNCTION(BlueprintCallable, Category = "Menu|GameOver")
	void ShowGameOverMenu();  // 게임 오버 창 띄우기

	void HideGameOverMenu();  // 게임 오버 창 숨기기

	// 메인로비 UI - 슈트 비활성화
	UFUNCTION(BlueprintCallable, Category = "NoPowerOnSuit")
	void ShowNoPowerOnSuit();
	UFUNCTION(BlueprintCallable, Category = "NoPowerOnSuit")
	void HideNoPowerOnSuit();

	// 인벤토리
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowInventory();  // 인벤토리 띄우기
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HideInventory();  // 인벤토리 숨기기

	// 동력코어
	UFUNCTION(BlueprintCallable, Category = "Item|PowerCore")
	void ShowItemPowerCore1();
	UFUNCTION(BlueprintCallable, Category = "Item|PowerCore")
	void ShowItemPowerCore2();
	
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

	void PlayEndingCredit(); // 엔딩크레딧 올라감
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|PowerCore")
	UUserWidget* ItemPowerCoreWidgetInstance1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|PowerCore")
	UUserWidget* ItemPowerCoreWidgetInstance2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NoPowerOnSuit")
	UUserWidget* NoPowerOnSuitWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EndingCredit")
	UUserWidget* EndingCreditWidgetInstance;

private:
	bool BossRoomEntered;  // 동력 코어 미션 완료 플래그 대체
	//bool CoreMFinished;  // 동력 코어 미션 완료 플래그
	bool BossMFinished;  // 보스 처치 미션 완료 플래그
};
