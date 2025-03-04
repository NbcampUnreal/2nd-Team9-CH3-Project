#include "0_Framework/MyGameState.h"

#include "2_AI/MeleeEnemyCharacter.h"
#include "1_UI/MyPlayerController.h"
#include "0_Framework/MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "1_UI/ScreenEffectComponent.h"
#include "1_UI/MyHUD.h"
#include "2_AI/SpawnEnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "1_UI/MyFunctionLibrary.h"
#include "EngineUtils.h"

AMyGameState::AMyGameState()
{
	CurrentLevelName = TEXT("");
	UILevelName = TEXT("");
	TotalSpawnedEnemyCount = 0;
	KillCount = 0;
	PowerCorePartsCount = 0;
	CurrentStage = 0;
	CoreMFinished = false;
	BossMFinished = false;
	CombatLogScrollBox = nullptr;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	//레벨 이동시, InputMode를 GameModeOnly로 초기화
	ResetInputMode();
	if (UWorld* World = GetWorld())
	{
		CurrentLevelName = FName(*World->GetName());
	}
	// 블루프린트 노드에 Fade In효과가 끝났을때 StartLevel()이 호출되게함.
	StartLevel();

	//적 생성 관련
	TArray<AActor*> FoundSpawnEnemyActors;	//스포너 찾아서 저장할 임시 배열, GetAllActorOfClass의 반환형이 AActor*이라서 이렇게 함
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnEnemyActor::StaticClass(), FoundSpawnEnemyActors);
	for (AActor* Actor : FoundSpawnEnemyActors)    //찾은 스포너를 ASpawnEnemyActor*로 캐스팅해서 EnemySpawners에 추가
	{
		ASpawnEnemyActor* Spawner = Cast<ASpawnEnemyActor>(Actor);
		if(Spawner)
		{
			EnemySpawners.Add(Spawner);
		}
	}
	SpawnEnemiesFromAllSpawners();	//EnemySpawners에 있는 모든 스포너에서 적 생성
	//현 레벨에 존재하는 적 수 카운트
	for(TActorIterator<AMeleeEnemyCharacter> It(GetWorld(), AMeleeEnemyCharacter::StaticClass()); It; ++It)
	{
		if(AMeleeEnemyCharacter* Enemy = Cast<AMeleeEnemyCharacter>(*It))
		{
			TotalSpawnedEnemyCount++;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Spawned Enemy : %d"), TotalSpawnedEnemyCount);
	

	// 영민 - 각 미션마다 체크 독립적으로 애니메이션 실행되는 지 확인하려고 잠깐 바꿈. 최종 시에 0.1f로 바꿔야 함.
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMyGameState::UpdateHUD,
		0.1f,
		true
	);

	// CombatLogScrollBox 값 설정
	if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))
	{
		if (UUserWidget* HUDWidgetInstance = HUD->GetHUDWidget())
		{
			CombatLogScrollBox = Cast<UScrollBox>(HUDWidgetInstance->GetWidgetFromName(TEXT("CombatLogBox")));

			if (!CombatLogScrollBox)
			{
				UE_LOG(LogTemp, Error, TEXT("CombatLogBox를 찾을 수 없습니다!"));
			}
		}
	}
}

void AMyGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			// 만약 메인 로비라면 HUD를 띄워주지 않음
			if (CurrentLevelName != TEXT("MainLobbyLevel"))  
			{
				// GetHUD()는 반환타입이 AHUD*라 직접 만든 AMyHUD* 타입으로 캐스팅해줘야 함.
				if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))  
				{
					MyHUD->ShowGameHUD();  // HUD에서 호출
				}

				// UI에 띄울 Level이름 설정
				if (CurrentLevelName == TEXT("StageLevel1"))
				{
					CurrentStage = 1;
					UILevelName = TEXT("생태 균형 실험실");
				}
				else if (CurrentLevelName == TEXT("StageLevel2"))
				{
					CurrentStage = 2;
					UILevelName = TEXT("식당");
				}
				else if (CurrentLevelName == TEXT("BossStageLevel"))
				{
					UILevelName = TEXT("심층 AI 알고리즘 핵심부");
				}
			}
			else
			{
				if (UScreenEffectComponent* ScreenEffect = MyPlayerController->ScreenEfc)
				{
					ScreenEffect->StartFadeIn(1.0f);  // ScreenEffectComponent에서 호출
				}
			}
			MyPlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AMyGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);  // 현재 레벨을 멈춘뒤..
			if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))  
			{
				MyHUD->ShowGameOverMenu();  // 게임 오버 메뉴 띄우기!
			}
		}
	}
}

void AMyGameState::ResetInputMode()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
			{
				MyPlayerController->SetInputMode(FInputModeGameOnly());
				MyPlayerController->bShowMouseCursor = false;
				MyPlayerController->SetPause(false);
			}
		}
	}
}

void AMyGameState::ShowJoinUI()
{
	if (!WBPJoinUI)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		if (!JoinUI)
		{
			JoinUI = CreateWidget<UUserWidget>(World, WBPJoinUI);
			if (JoinUI)
			{
				JoinUI->AddToViewport();
			}

			if (UTextBlock* UIText = Cast<UTextBlock>(JoinUI->GetWidgetFromName(TEXT("UIText"))))
			{
				if (!TargetLevelName.IsNone())
				{
					if (TargetLevelName == TEXT("MainLobbyLevel"))
					{
						UIText->SetText(FText::FromString(TEXT("[메인 로비]로 이동하시겠습니까?")));
					}
					else if (TargetLevelName == TEXT("StageLevel1"))
					{
						UIText->SetText(FText::FromString(TEXT("[생태 균형 실험실]으로 이동하시겠습니까?")));
					}
					else if (TargetLevelName == TEXT("StageLevel2"))
					{
						UIText->SetText(FText::FromString(TEXT("[식당]으로 이동하시겠습니까?")));
					}
					else if (TargetLevelName == TEXT("BossStageLevel"))
					{
						UIText->SetText(FText::FromString(TEXT("[심층 AI 알고리즘 핵심부]으로 이동하시겠습니까?")));
					}
				}
			}
			
		}
		else if (!JoinUI->IsInViewport())
		{
			JoinUI->AddToViewport();
		}

		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
			{
				MyPlayerController->SetInputMode(FInputModeUIOnly());
				MyPlayerController->bShowMouseCursor = true;
				MyPlayerController->SetPause(true);
			}
		}
	}
}


void AMyGameState::HideJoinUI()
{
	if (JoinUI)
	{
		JoinUI->RemoveFromParent();
		JoinUI = nullptr;
		ResetInputMode();
	}
}

void AMyGameState::SetTargetLevelName(FName NewLevelName)
{
	TargetLevelName = NewLevelName;
}

void AMyGameState::ConfirmMoveLevel()
{
	HideJoinUI();
	UGameplayStatics::OpenLevel(this, TargetLevelName);
}

void AMyGameState::DeclineMoveLevel()
{
	HideJoinUI();
	TargetLevelName = TEXT("");
}

// UpdateHUD()를 구현할 때 GameState의 멤버변수들을 쓸일이 많을거 같아서 HUD가 아닌 GameState에서 구현!
void AMyGameState::UpdateHUD()
{
	/*UWorld* World = GetWorld();
	if (!World) return;*/

	if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))
	{
		HUD->UpdateCharacterHPBar();
		HUD->UpdateMission();

		if (PowerCorePartsCount != 2)
		{
			PowerCorePartsCount++;
		}
		else if (!CoreMFinished)
		{
			HUD->PlayAnimCoreMFinished();
			CoreMFinished = true;
		}

		if (PowerCorePartsCount == 1)
		{
			HUD->PlayAnimBossMFinished();
		}

		if (UUserWidget* HUDWidgetInstance = HUD->GetHUDWidget())
		{
			if (UTextBlock* StageText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("StageText"))))
			{
				// FString타입의 UILevelName을 %s에 넣기 위해선 const Tchar* 타입으로 변환이 필요함. 그래서 *을 붙여야 함.
				StageText->SetText(FText::FromString(FString::Printf(TEXT("스테이지 %d %s"), CurrentStage, *UILevelName)));
			}

			if (UTextBlock* KillEnemyText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("KillEnemy"))))
			{
				KillEnemyText->SetText(FText::FromString(FString::Printf(TEXT("적 처치 %d / %d"), 0, TotalSpawnedEnemyCount)));
			}
		}
	}
}

void AMyGameState::SpawnEnemiesFromAllSpawners()
{
	for(ASpawnEnemyActor* Spawner : EnemySpawners)
	{
		if (Spawner)
		{
			Spawner->SpawnEnemy();
		}
	}
}

FName AMyGameState::GetCurrentLevelName()
{
	return CurrentLevelName;
}

int32 AMyGameState::GetPowerCorePartsCount()
{
	return PowerCorePartsCount;
}

// 전투 로그 메시지를 보여주는 함수
void AMyGameState::AddCombatLogMessage(const FString& NewMessage)
{
	if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))
	{
		if (UUserWidget* CombatLogWidgetInstance = HUD->GetCombatLogWidget())
		{
			if (UTextBlock* NewCombatLogText = Cast<UTextBlock>(CombatLogWidgetInstance->GetWidgetFromName(TEXT("CombatLogText"))))
			{
				HUD->CreateCombatLogWidget();

				NewCombatLogText->SetText(FText::FromString(NewMessage));
				NewCombatLogText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));

				CombatLogScrollBox->AddChild(CombatLogWidgetInstance);
				CombatLogScrollBox->ScrollToEnd();  // ScrollBox의 내장 함수

				CombatLogWidgetArray.Add(CombatLogWidgetInstance); // 배열에 저장

				FTimerHandle LogEndTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(
					LogEndTimerHandle,
					this,
					&AMyGameState::RemoveOldCombatLogWidget,
					2.0f,
					false
				);
			}
		}
	}
}

void AMyGameState::RemoveOldCombatLogWidget()
{
	if (CombatLogWidgetArray.Num() > 0)  // 배열에 로그가 존재할때만 삭제
	{
		UUserWidget* WidgetToRemove = CombatLogWidgetArray[0];

		if (CombatLogScrollBox && WidgetToRemove)
		{
			CombatLogScrollBox->RemoveChild(WidgetToRemove); // ScrollBox에서 삭제
		}

		CombatLogWidgetArray.RemoveAt(0); // 배열에서도 제거
	}
}
