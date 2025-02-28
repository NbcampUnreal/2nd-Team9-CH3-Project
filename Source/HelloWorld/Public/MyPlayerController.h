#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class HELLOWORLD_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController(); // 생성자

	// 멤버 변수 - UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu|Main")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu|GameOver")
	TSubclassOf<UUserWidget> GameOverMenuWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	TSubclassOf<UUserWidget> FadeInAndOutWidgetClass;

	// 메서드 - UI 관련 
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();  // 전투 중 게임에 실시간으로 보여질 UI 띄우기
	
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void ShowMainMenu();  // 게임 시작 메뉴창 띄우기
	
	UFUNCTION(BlueprintCallable, Category = "Menu|GameOver")
	void ShowGameOverMenu(); // 게임 오버 메뉴창 띄우기
	
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeIn();  // 레벨에 진입 시 서서히 화면이 밝아지는 효과
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeOut();  // 레벨이 변경되기 전 서서히 화면이 어두워지는 효과
	
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void StartGame(int32 LevelIndex);  // 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu|Main")
	void QuitGame();  // 게임 종료
	
	UFUNCTION(BlueprintCallable, Category = "Menu|GameOver")
	void ReStartStage();  // 스테이지 재시작
	
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu|Main")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu|GameOver")
	UUserWidget* GameOverMenuWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fade")
	UUserWidget* FadeInAndOutWidgetInstance;
};
