#include "0_Framework/MyGameState.h"

#include "2_AI/MeleeEnemyCharacter.h"
#include "1_UI/MyPlayerController.h"
#include "0_Framework/MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "1_UI/ScreenEffectComponent.h"
#include "1_UI/MyHUD.h"
#include "2_AI/SpawnEnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "1_UI/MyFunctionLibrary.h"
#include "EngineUtils.h"
#include "SWarningOrErrorBox.h"
#include "0_Framework/MyGameMode.h"

AMyGameState::AMyGameState()
{
	JoinUI = nullptr;
	CurrentArtifactCount = 0;
	CurrentLevelName = TEXT("");
	UILevelName = TEXT("");
	TotalSpawnedEnemyCount = 0;
	KillCount = 0;
	PowerCorePartsCount = 0;
	MaxPowerCoreParts = 2;
	CurrentStage = 0;
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
	//적 소환
	SpawnEnemiesInLevel(); //StartLevel() 함수 안에 넣어버리는 것도 괜찮을 것 같음 - 규혁


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

	WeaponImage1 = LoadObject<UTexture2D>(nullptr, TEXT("/Game/_UI/_UI_Textures/Weapon1.Weapon1"));
	WeaponImage2 = LoadObject<UTexture2D>(nullptr, TEXT("/Game/_UI/_UI_Textures/Weapon2.Weapon2"));
}

void AMyGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UScreenEffectComponent* ScreenEffect = MyPlayerController->ScreenEfc)
			{
				ScreenEffect->StartFadeIn(3.0f); // ScreenEffectComponent에서 호출
			}
			// 만약 메인 로비라면 HUD를 띄워주지 않음
			if (CurrentLevelName != TEXT("MainLobbyLevel"))
			{
				// GetHUD()는 반환타입이 AHUD*라 직접 만든 AMyHUD* 타입으로 캐스팅해줘야 함.
				if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))
				{
					MyHUD->ShowGameHUD(); // HUD에서 호출
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
					CurrentStage = 99;
					UILevelName = TEXT("심층 AI 알고리즘 핵심부");
				}
			}
			MyPlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AMyGameState::EndLevel()
{
	if (AMyGameMode* MyGameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		MyGameMode->ExitLevel();
	}
}

void AMyGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true); // 현재 레벨을 멈춘뒤..
			if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))
			{
				MyHUD->ShowGameOverMenu(); // 게임 오버 메뉴 띄우기!
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
	if (!WBPJoinUI || !WBPJoinUI_Boss)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		if (!JoinUI)
		{
			if (!TargetLevelName.IsNone())
			{
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(World->GetGameInstance()))
				{
					PowerCorePartsCount = MyGameInstance->GetPowerCoreCount();
				}
				if (TargetLevelName == TEXT("BossStageLevel") && PowerCorePartsCount < MaxPowerCoreParts)
				{
					UE_LOG(LogTemp, Warning, TEXT("보스방, 동력코어 부족 UI 조건문 걸림"));
					JoinUI = CreateWidget<UUserWidget>(World, WBPJoinUI_Boss);
					if (JoinUI)
					{
						JoinUI->AddToViewport();
						if (UTextBlock* UIText = Cast<UTextBlock>(JoinUI->GetWidgetFromName(TEXT("UIText2"))))
						{
							UIText->SetText(FText::FromString(FString::Printf(TEXT("동력 코어 부족 [%d / %d]"), PowerCorePartsCount, MaxPowerCoreParts)));
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("보스방, 동력코어 부족 UI 조건문 else로 나옴"));
					JoinUI = CreateWidget<UUserWidget>(World, WBPJoinUI);
					if (JoinUI)
					{
						JoinUI->AddToViewport();
						if (UTextBlock* UIText = Cast<UTextBlock>(JoinUI->GetWidgetFromName(TEXT("UIText"))))
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
							else if (TargetLevelName == TEXT("BossStageLevel") && PowerCorePartsCount >= MaxPowerCoreParts)
							{
								UIText->SetText(FText::FromString(TEXT("[심층 AI 알고리즘 핵심부]으로 이동하시겠습니까?")));
							}
						}
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
	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		MyGameInstance->MarkTriggerBoxAsUsed(UsedTriggerBox);
	}
	HideJoinUI();
	EndLevel();
	// 레벨 열기전에 FadeOut 효과
	UMyFunctionLibrary::StartFadeOut(this);
	float FadeOutDuration = UMyFunctionLibrary::GetFadeDuration(this);

	GetWorld()->GetTimerManager().SetTimer(
		FadeOutTimer,
		this,
		&AMyGameState::OpenTargetLevel,
		FadeOutDuration,
		false
	);
}

void AMyGameState::OpenTargetLevel()
{
	UGameplayStatics::OpenLevel(this, TargetLevelName);
}

void AMyGameState::DeclineMoveLevel()
{
	UsedTriggerBox = "";
	HideJoinUI();
	TargetLevelName = TEXT("");
}

// GameState에서 업데이트 되는 정보 관련 함수----------------------------------------------
// UpdateHUD()를 구현할 때 GameState의 멤버변수들을 쓸일이 많을거 같아서 HUD가 아닌 GameState에서 구현!
void AMyGameState::UpdateHUD()
{
	if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))
	{
		HUD->UpdateCharacterHPBar();
		HUD->UpdateMission();


		if (UUserWidget* HUDWidgetInstance = HUD->GetHUDWidget())
		{
			if (UTextBlock* StageText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("StageText"))))
			{
				if (CurrentStage != 99)
				{
					// FString타입의 UILevelName을 %s에 넣기 위해선 const Tchar* 타입으로 변환이 필요함. 그래서 *을 붙여야 함.
					StageText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *UILevelName)));
				}
				else
				{
					StageText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *UILevelName)));
					StageText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

					if (UTextBlock* KillText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("KillEnemy"))))
					{
						KillText->SetVisibility(ESlateVisibility::Hidden);
					}

					if (UUserWidget* BossHPBar = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("WBP_Boss_HP_Bar"))))
					{
						BossHPBar->SetVisibility(ESlateVisibility::Visible);
					}
				}
			}

			if (UTextBlock* KillEnemyText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("KillEnemy"))))
			{
				if (UWidget* TutorialInformationBox = Cast<UWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("TutorialInformation"))))
				{
					if (CurrentLevelName == "TutorialLevel")
					{
						KillEnemyText->SetVisibility(ESlateVisibility::Hidden);
						TutorialInformationBox->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						KillEnemyText->SetText(
							FText::FromString(FString::Printf(TEXT("적 처치 %d / %d"), KillCount, TotalSpawnedEnemyCount)));

						TutorialInformationBox->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			}
		}
	}
}

void AMyGameState::SetMainGunImage(UTexture2D* NewTexture)
{
	if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))
	{
		if (UUserWidget* HUDWidgetInstance = HUD->GetHUDWidget())
		{
			if (UImage* MainGunImage = Cast<UImage>(HUDWidgetInstance->GetWidgetFromName(TEXT("MainGun"))))
			{
				if (NewTexture)
				{
					MainGunImage->SetBrushFromTexture(NewTexture);
				}
			}
		}
	}
}

void AMyGameState::AddKillCount()
{
	if (CurrentLevelName == "StageLevel1" || CurrentLevelName == "StageLevel2")
	{
		KillCount++;
		if (KillCount >= TotalSpawnedEnemyCount)
		{
			UpdateHUD();
			OnAllEnemiesKilled.Broadcast();
		}
	}
}
// GameState에서 업데이트 되는 정보 관련 함수 끝----------------------------------------------

//BeginPlay에서 호출
void AMyGameState::SpawnEnemiesInLevel()
{
	//적 스폰하는 레벨 아니면 리턴
	if (CurrentLevelName != TEXT("StageLevel1") && CurrentLevelName != TEXT("StageLevel2"))
	{
		return;
	}

	TArray<AActor*> FoundSpawnEnemyActors; //스포너 찾아서 저장할 임시 배열, GetAllActorOfClass의 반환형이 AActor*이라서 이렇게 함
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnEnemyActor::StaticClass(), FoundSpawnEnemyActors);
	for (AActor* Actor : FoundSpawnEnemyActors) //찾은 스포너를 ASpawnEnemyActor*로 캐스팅해서 EnemySpawners에 추가
	{
		ASpawnEnemyActor* Spawner = Cast<ASpawnEnemyActor>(Actor);
		if (Spawner)
		{
			EnemySpawners.Add(Spawner);
		}
	}

	//모든 스포너에서 적 생성
	for (ASpawnEnemyActor* Spawner : EnemySpawners)
	{
		if (Spawner)
		{
			Spawner->SpawnEnemy();
		}
	}

	//현 레벨에 존재하는 적 수 카운트
	for (TActorIterator<AMeleeEnemyCharacter> It(GetWorld(), AMeleeEnemyCharacter::StaticClass()); It; ++It)
	{
		if (AMeleeEnemyCharacter* Enemy = Cast<AMeleeEnemyCharacter>(*It))
		{
			TotalSpawnedEnemyCount++;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("총 스폰된 적 수 %d"), TotalSpawnedEnemyCount);
}

FName AMyGameState::GetCurrentLevelName() const
{
	return CurrentLevelName;
}

int32 AMyGameState::GetPowerCorePartsCount() const
{
	return PowerCorePartsCount;
}

int32 AMyGameState::GetKillCount() const
{
	return KillCount;
}

void AMyGameState::SetUsedTriggerBox(FName Target)
{
	UsedTriggerBox = Target;
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
