#include "0_Framework/MyGameState.h"

#include "2_AI/MeleeEnemyCharacter.h"
#include "1_UI/MyPlayerController.h"
#include "0_Framework/MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "1_UI/ScreenEffectComponent.h"
#include "1_UI/MyHUD.h"
#include "2_AI/SpawnEnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

AMyGameState::AMyGameState()
{
	JoinUI = nullptr;
	CurrentArtifactCount = 0;
	CurrentLevelName = TEXT("");
	TotalSpawnedEnemyCount = 0;
	PowerCoreCount = 0;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateDataFromInstance();
	
	//레벨 이동시, InputMode를 GameModeOnly로 초기화
	ResetInputMode();
	if (UWorld* World = GetWorld())
	{
		CurrentLevelName = FName(*World->GetName());
	}
	// 블루프린트 노드에 Fade In효과가 끝났을때 StartLevel()이 호출되게함.
	StartLevel();
	//적 소환
	SpawnEnemiesInLevel();	//StartLevel() 함수 안에 넣어버리는 것도 괜찮을 것 같음 - 규혁
	
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMyGameState::UpdateHUD,
		0.1f,
		true
	);
}

//BeginPlay()에서 호출
void AMyGameState::UpdateDataFromInstance()
{
	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		PowerCoreCount = MyGameInstance->GetPowerCoreCount();
		//추후 데이터 추가된다면 밑에 추가
		//
		//
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

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))  
			{
				if (UUserWidget* HUDWidget = MyHUD->GetHUDWidget())  
				{
					if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
					{
						
					}
				}
			}
		}
	}
}

//BeginPlay에서 호출
void AMyGameState::SpawnEnemiesInLevel()
{
	//적 스폰하는 레벨 아니면 리턴
	if (CurrentLevelName != TEXT("StageLevel1") && CurrentLevelName != TEXT("StageLevel2"))
	{
		return;
	}
	
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

	//모든 스포너에서 적 생성
	for(ASpawnEnemyActor* Spawner : EnemySpawners)
	{
		if (Spawner)
		{
			Spawner->SpawnEnemy();
		}
	}
	
	//현 레벨에 존재하는 적 수 카운트
	for(TActorIterator<AMeleeEnemyCharacter> It(GetWorld(), AMeleeEnemyCharacter::StaticClass()); It; ++It)
	{
		if(AMeleeEnemyCharacter* Enemy = Cast<AMeleeEnemyCharacter>(*It))
		{
			TotalSpawnedEnemyCount++;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Spawned Enemy : %d"), TotalSpawnedEnemyCount);
}

FName AMyGameState::GetCurrentLevelName() const
{
	return CurrentLevelName;
}