#include "5_Sound/DialogueSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    
	// 초기화 코드
	UDataTable* DialogueTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Sound/SupAIDataTable"));
	if (DialogueTable)
	{
		LoadDialogueDataTable(DialogueTable);
		UE_LOG(LogTemp, Log, TEXT("Loaded default dialogue table"));
	}
	
	UE_LOG(LogTemp, Log, TEXT("DialogueSubsystem initialized"));
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
}


void UDialogueSubsystem::SetDialogueData(EDialogueSupAI DialogueType, const FDialogueDataSupAI& DialogueData)
{
	DialogueSupAIMap[DialogueType] = DialogueData;
	UE_LOG(LogTemp, Log, TEXT("Added dialogue data for: %s"), *UEnum::GetValueAsString(DialogueType));
}

void UDialogueSubsystem::SetDialogueDataMap(const TMap<EDialogueSupAI, FDialogueDataSupAI>& InDialogueMap)
{
	DialogueSupAIMap = InDialogueMap;
	UE_LOG(LogTemp, Log, TEXT("Set dialogue data map with %d entries"), DialogueSupAIMap.Num());
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

		if (CurrentTime > LastTime + MinTimeBetween)
		{
			UE_LOG(LogTemp, Verbose, TEXT("Cannot play dialogue %s yet. Need to wait %f more seconds."), 
				*UEnum::GetValueAsString(DialogueType), 
				MinTimeBetween - (CurrentTime - LastTime));
			return false;
		}
	}
	
	return true;
}