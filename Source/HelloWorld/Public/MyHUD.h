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
	
	// 메서드 - UI 관련 
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();  // 전투 중 게임에 실시간으로 보여질 UI 띄우기
	
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void ShowMainMenu();  // 게임 시작 메뉴창 띄우기
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void HideMainMenu();  // 게임 시작 메뉴창 숨기기
	UFUNCTION(BlueprintCallable, Category = "Menu|GameOver")
	void ShowGameOverMenu(); // 게임 오버 메뉴창 띄우기
	
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeIn(float Duration);  // 레벨에 진입 시 서서히 화면이 밝아지는 효과
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeOut(float Duration);  // 레벨이 변경되기 전 서서히 화면이 어두워지는 효과
	
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void StartGame();  // 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void QuitGame();  // 게임 종료
	UFUNCTION(BlueprintCallable, Category = "Menu|GameOver")
	void ReStartStage();  // 스테이지 재시작

protected:
	// 멤버 변수 - UI 클래스
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu|Main")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu|GameOver")
	UUserWidget* GameOverMenuWidgetInstance;
};
