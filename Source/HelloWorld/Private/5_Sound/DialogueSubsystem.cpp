#include "5_Sound/DialogueSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// FTimerHandle LoadDelayTimer;
	// GetWorld()->GetTimerManager().SetTimer(
	// 	LoadDelayTimer,
	// 	this,
	// 	&UDialogueSubsystem::LoadDataTables,
	// 	0.1f,  // 0.1초 후에 로드
	// 	false
	// );
}

void UDialogueSubsystem::LoadDataTables()
{
	//SupAI
	UDataTable* DialogueTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Sound/SupAIDataTable"));
	if (DialogueTable)
	{
		LoadDialogueDataTable(DialogueTable);
		UE_LOG(LogTemp, Log, TEXT("Loaded default dialogue table"));
	}
	
	//BossAI 데이터 테이블 로드
	UDataTable* BossDialogueTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Sound/BossAIDataTable"));
	if (BossDialogueTable)
	{
		LoadBossDialogueDataTable(BossDialogueTable);
		UE_LOG(LogTemp, Warning, TEXT("Loaded BossAI dialogue table"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load BossAI dialogue table"));
	}
	UE_LOG(LogTemp, Log, TEXT("DialogueSubsystem initialized"));
}

void UDialogueSubsystem::StopCurrentDialogue()
{
	if (CurrentDialogue && CurrentDialogue->IsPlaying())
	{
		CurrentDialogue->Stop();
		CurrentDialogue = nullptr;
	}
}

void UDialogueSubsystem::Deinitialize()
{
	if (CurrentDialogue && CurrentDialogue->IsPlaying())
	{
		CurrentDialogue->Stop();
	}
	CurrentDialogue = nullptr;
	CurrentSubtitle = FString();
	
	Super::Deinitialize();
}

void UDialogueSubsystem::PlaySupAIDialogue(EDialogueSupAI DialogueType)
{
	if (!CanPlayDialogue(DialogueType))
	{
		return;
	}

	if (DialogueSupAIMap.Contains(DialogueType))
	{
		const FDialogueDataSupAI& DialogueData = DialogueSupAIMap[DialogueType];

		//이미 재생중인 것 있으면 중지
		if (CurrentDialogue && CurrentDialogue->IsPlaying())
		{
			CurrentDialogue->Stop();
			CurrentDialogue = nullptr;
		}

		//재생
		if (DialogueData.DialogueAudio)
		{
			CurrentDialogue = UGameplayStatics::SpawnSound2D(
				GetWorld(),
				DialogueData.DialogueAudio,
				1.0f,
				1.0f,
				0.0f,
				nullptr,
				true,
				false
				);
		}

		CurrentSubtitle = DialogueData.SubtitleText;
		LastPlayBackTimes.Add(DialogueType, GetWorld()->GetTimeSeconds());
		
		// 로그 출력
		UE_LOG(LogTemp, Log, TEXT("Playing SupAI dialogue: %s"), *UEnum::GetValueAsString(DialogueType));
	}
}

void UDialogueSubsystem::PlayBossAIDialogue(EDialogueBossAI DialogueType)
{
    UE_LOG(LogTemp, Warning, TEXT("Attempting to play BossAI dialogue: %s"), *UEnum::GetValueAsString(DialogueType));
    
    if (!CanPlayBossDialogue(DialogueType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot play BossAI dialogue due to time constraints: %s"), 
            *UEnum::GetValueAsString(DialogueType));
        return;
    }

    if (DialogueBossAIMap.Contains(DialogueType))
    {
        const FDialogueDataBossAI& DialogueData = DialogueBossAIMap[DialogueType];

        // 이미 재생중인 것 있으면 중지
        if (CurrentDialogue && CurrentDialogue->IsPlaying())
        {
            CurrentDialogue->Stop();
            CurrentDialogue = nullptr;
        }

    	CurrentBossDialogueType = DialogueType;
        // 재생
        if (DialogueData.DialogueAudio)
        {
            CurrentDialogue = UGameplayStatics::SpawnSound2D(
                GetWorld(),
                DialogueData.DialogueAudio,
                1.0f,
                1.0f,
                0.0f,
                nullptr,
                true,
                false
            );

        	float Duration = DialogueData.AudioDuration;
        	GetWorld()->GetTimerManager().SetTimer(
        		DialogueFinishTimerHandle,
        		this,
        		&UDialogueSubsystem::HandleDialogueFinished,
        		Duration,
        		false
        		);
        	
            if (CurrentDialogue)
            {
                UE_LOG(LogTemp, Warning, TEXT("Successfully spawned BossAI sound for: %s"), 
                    *UEnum::GetValueAsString(DialogueType));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn BossAI sound for: %s"), 
                    *UEnum::GetValueAsString(DialogueType));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No audio file for BossAI dialogue: %s"), 
                *UEnum::GetValueAsString(DialogueType));
        }

        CurrentSubtitle = DialogueData.SubtitleText;
        LastBossPlayBackTimes.Add(DialogueType, GetWorld()->GetTimeSeconds());
        
        UE_LOG(LogTemp, Log, TEXT("Playing BossAI dialogue: %s"), *UEnum::GetValueAsString(DialogueType));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BossAI dialogue type not found in map: %s"), 
            *UEnum::GetValueAsString(DialogueType));
    }
}

void UDialogueSubsystem::PlaySupAIDialogueSequence(const TArray<EDialogueSupAI>& DialogueSequence)
{
	// 이미 재생 중인 시퀀스가 있으면 취소
	if (SequenceTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SequenceTimerHandle);
	}

	// 새 시퀀스 설정
	CurrentDialogueSequence = DialogueSequence;
	CurrentSequenceIndex = 0;

	if (CurrentDialogueSequence.Num() > 0)
	{
		EDialogueSupAI CurrentType = CurrentDialogueSequence[CurrentSequenceIndex];
		PlaySupAIDialogue(CurrentType);

		float CurrentDuration = 2.0f; //Default 값 : 2.0f
		if (DialogueSupAIMap.Contains(CurrentType))
		{
			CurrentDuration = DialogueSupAIMap[CurrentType].AudioDuration + 0.2f;	//여유 시간 살짝 추가
		}
		
		// 다음 대사를 위한 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			SequenceTimerHandle,
			this,
			&UDialogueSubsystem::PlayNextInSequence,
			CurrentDuration,
			false
		);
	}
}

void UDialogueSubsystem::PlayBossAIDialogueSequence(const TArray<EDialogueBossAI>& DialogueSequence)
{
	UE_LOG(LogTemp, Warning, TEXT("Starting BossAI dialogue sequence with %d entries"), DialogueSequence.Num());
    
	// 이미 재생 중인 시퀀스가 있으면 취소
	if (BossSequenceTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(BossSequenceTimerHandle);
	}

	// 새 시퀀스 설정
	CurrentBossDialogueSequence = DialogueSequence;
	CurrentBossSequenceIndex = 0;

	if (CurrentBossDialogueSequence.Num() > 0)
	{
		EDialogueBossAI CurrentType = CurrentBossDialogueSequence[CurrentBossSequenceIndex];
		PlayBossAIDialogue(CurrentType);

		float CurrentDuration = 2.0f; // Default 값 : 2.0f
		if (DialogueBossAIMap.Contains(CurrentType))
		{
			CurrentDuration = DialogueBossAIMap[CurrentType].AudioDuration + 0.2f; // 여유 시간 살짝 추가
		}
        
		// 다음 대사를 위한 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			BossSequenceTimerHandle,
			this,
			&UDialogueSubsystem::PlayNextBossInSequence,
			CurrentDuration,
			false
		);
	}
}

void UDialogueSubsystem::PlayNextInSequence()
{
	CurrentSequenceIndex++;

	if (CurrentSequenceIndex < CurrentDialogueSequence.Num())
	{
		EDialogueSupAI CurrentType = CurrentDialogueSequence[CurrentSequenceIndex];
		PlaySupAIDialogue(CurrentType);

		// 데이터테이블에서 현재 대사의 길이 가져오기
		float CurrentDuration = 2.0f; //Default 값 : 2.0f
		if (DialogueSupAIMap.Contains(CurrentType))
		{
			CurrentDuration = DialogueSupAIMap[CurrentType].AudioDuration + 0.2f;	//여유 시간 살짝 추가
		}

		// 다음 대사를 위한 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			SequenceTimerHandle,
			this,
			&UDialogueSubsystem::PlayNextInSequence,
			CurrentDuration,
			false
		);
	}
	else
	{
		OnDialogueSupAIFinished.Broadcast(CurrentDialogueSupAIType);
	}
}

void UDialogueSubsystem::PlayNextBossInSequence()
{
	CurrentBossSequenceIndex++;

	if (CurrentBossSequenceIndex < CurrentBossDialogueSequence.Num())
	{
		EDialogueBossAI CurrentType = CurrentBossDialogueSequence[CurrentBossSequenceIndex];
		PlayBossAIDialogue(CurrentType);

		// 데이터테이블에서 현재 대사의 길이 가져오기
		float CurrentDuration = 2.0f; // Default 값 : 2.0f
		if (DialogueBossAIMap.Contains(CurrentType))
		{
			CurrentDuration = DialogueBossAIMap[CurrentType].AudioDuration + 0.2f; // 여유 시간 살짝 추가
		}

		// 다음 대사를 위한 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			BossSequenceTimerHandle,
			this,
			&UDialogueSubsystem::PlayNextBossInSequence,
			CurrentDuration,
			false
		);
	}
}


void UDialogueSubsystem::SetDialogueData(EDialogueSupAI DialogueType, const FDialogueDataSupAI& DialogueData)
{
	DialogueSupAIMap[DialogueType] = DialogueData;
	UE_LOG(LogTemp, Log, TEXT("Added dialogue data for: %s"), *UEnum::GetValueAsString(DialogueType));
}

void UDialogueSubsystem::SetBossDialogueData(EDialogueBossAI DialogueType, const FDialogueDataBossAI& DialogueData)
{
	DialogueBossAIMap[DialogueType] = DialogueData;
	UE_LOG(LogTemp, Log, TEXT("Added BossAI dialogue data for: %s"), *UEnum::GetValueAsString(DialogueType));
}

void UDialogueSubsystem::SetDialogueDataMap(const TMap<EDialogueSupAI, FDialogueDataSupAI>& InDialogueMap)
{
	DialogueSupAIMap = InDialogueMap;
	UE_LOG(LogTemp, Log, TEXT("Set dialogue data map with %d entries"), DialogueSupAIMap.Num());
}

void UDialogueSubsystem::SetBossDialogueDataMap(const TMap<EDialogueBossAI, FDialogueDataBossAI>& InDialogueMap)
{
	DialogueBossAIMap = InDialogueMap;
	UE_LOG(LogTemp, Log, TEXT("Set BossAI dialogue data map with %d entries"), DialogueBossAIMap.Num());
}

bool UDialogueSubsystem::IsPlayingDialogue() const
{
	return CurrentDialogue && CurrentDialogue->IsPlaying();
}

FString UDialogueSubsystem::GetCurrentSubtitle() const
{
	return CurrentSubtitle;
}

//데이터테이블 읽어와서 DialogueSupAIMap에 저장
void UDialogueSubsystem::LoadDialogueDataTable(UDataTable* DataTable)
{
	if (!DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid DataTable provided to LoadDialogueDataTable"));
		return;
	}

	TArray<FDialogueDataSupAITableRow*> Rows;
	DataTable->GetAllRows<FDialogueDataSupAITableRow>(TEXT("DialogueSubsystem"), Rows);

	for (FDialogueDataSupAITableRow* Row : Rows)
	{
		if (Row)
		{
			DialogueSupAIMap.Add(Row->DialogueT, Row->DialogueD);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Loaded %d dialogue entries from data table"), Rows.Num());
}

void UDialogueSubsystem::LoadBossDialogueDataTable(UDataTable* DataTable)
{
	if (!DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid DataTable provided to LoadBossDialogueDataTable"));
		return;
	}

	TArray<FDialogueDataBossAITableRow*> Rows;
	DataTable->GetAllRows<FDialogueDataBossAITableRow>(TEXT("DialogueSubsystem"), Rows);

	for (FDialogueDataBossAITableRow* Row : Rows)
	{
		if (Row)
		{
			DialogueBossAIMap.Add(Row->DialogueT, Row->DialogueD);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded %d BossAI dialogue entries from data table"), Rows.Num());
}

bool UDialogueSubsystem::CanPlayDialogue(EDialogueSupAI DialogueType)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	//이전 재생 확인
	if (LastPlayBackTimes.Contains(DialogueType))
	{
		float LastTime = LastPlayBackTimes[DialogueType];
		float MinTimeBetween = 5.0f;

		//설정된 최소 시간 있으면 사용
		if (DialogueSupAIMap.Contains(DialogueType))
		{
			MinTimeBetween = DialogueSupAIMap[DialogueType].MinTimeBetweenPlaybacks;
		}

		if (CurrentTime < LastTime + MinTimeBetween)
		{
			UE_LOG(LogTemp, Verbose, TEXT("Cannot play dialogue %s yet. Need to wait %f more seconds."), 
				*UEnum::GetValueAsString(DialogueType), 
				MinTimeBetween - (CurrentTime - LastTime));
			return false;
		}
	}
	
	return true;
}

bool UDialogueSubsystem::CanPlayBossDialogue(EDialogueBossAI DialogueType)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// 이전 재생 확인
	if (LastBossPlayBackTimes.Contains(DialogueType))
	{
		float LastTime = LastBossPlayBackTimes[DialogueType];
		float MinTimeBetween = 5.0f;

		// 설정된 최소 시간 있으면 사용
		if (DialogueBossAIMap.Contains(DialogueType))
		{
			MinTimeBetween = DialogueBossAIMap[DialogueType].MinTimeBetweenPlaybacks;
		}

		// 조건문 수정: 현재 시간이 (마지막 재생 시간 + 최소 간격)보다 작으면 재생 불가
		if (CurrentTime < LastTime + MinTimeBetween)
		{
			UE_LOG(LogTemp, Verbose, TEXT("Cannot play BossAI dialogue %s yet. Need to wait %f more seconds."), 
				*UEnum::GetValueAsString(DialogueType), 
				(LastTime + MinTimeBetween) - CurrentTime);
			return false;
		}
	}
    
	return true;
}

void UDialogueSubsystem::HandleDialogueFinished()
{
	OnDialogueFinished.Broadcast(CurrentBossDialogueType);
	CurrentBossDialogueType = EDialogueBossAI::None;
}
