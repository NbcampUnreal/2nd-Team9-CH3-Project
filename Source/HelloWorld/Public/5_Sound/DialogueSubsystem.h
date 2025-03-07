#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "SupAIDialogueType.h"
#include "BossAIDialogueType.h"
#include "DialogueSubsystem.generated.h"

UCLASS()
class HELLOWORLD_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueFinished, EDialogueBossAI, DialogueType);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueSupAIFinished, EDialogueSupAI, DialogueType);

	UPROPERTY()
	FOnDialogueFinished OnDialogueFinished;
	UPROPERTY()
	FOnDialogueSupAIFinished OnDialogueSupAIFinished;
	//SupAI 대사 재생
	UFUNCTION(BlueprintCallable, Category = "Dialogue|SupportAI")
	void PlaySupAIDialogue(EDialogueSupAI DialogueType);
	UFUNCTION(BlueprintCallable, Category = "Dialogue|SupportAI")
	void PlaySupAIDialogueSequence(const TArray<EDialogueSupAI>& DialogueSequence);
	//BossAI 대사 재생
	UFUNCTION(BlueprintCallable, Category = "Dialogue|BossAI")
	void PlayBossAIDialogue(EDialogueBossAI DialogueType);
	UFUNCTION(BlueprintCallable, Category = "Dialogue|BossAI")
	void PlayBossAIDialogueSequence(const TArray<EDialogueBossAI>& DialogueSequence);
	
	//SupAI 대사 데이터 설정
	UFUNCTION(BlueprintCallable, Category = "Dialogue|SupportAI")
	void SetDialogueData(EDialogueSupAI DialogueType, const FDialogueDataSupAI& DialogueData);
	//BossAI 대사 데이터 설정
	UFUNCTION(BlueprintCallable, Category = "Dialogue|BossAI")
	void SetBossDialogueData(EDialogueBossAI DialogueType, const FDialogueDataBossAI& DialogueData);
	//현재 재생 중인지 확인
	UFUNCTION(BlueprintPure, Category = "Dialogue")
	bool IsPlayingDialogue() const;
	//현재 자막 가져오기
	UFUNCTION(BlueprintPure, Category = "Dialogue")
	FString GetCurrentSubtitle() const;
	//SupAI 대사 데이터맵 가져오기
	UFUNCTION(BlueprintPure, Category = "Dialogue|SupportAI")
	void SetDialogueDataMap(const TMap<EDialogueSupAI, FDialogueDataSupAI>& InDialogueMap);
	//BossAI 대사 데이터맵 설정
	UFUNCTION(BlueprintCallable, Category = "Dialogue|BossAI")
	void SetBossDialogueDataMap(const TMap<EDialogueBossAI, FDialogueDataBossAI>& InDialogueMap);
	//SupAI 데이터 테이블 로드
	UFUNCTION(BlueprintCallable, Category = "Dialogue|SupportAI")
	void LoadDialogueDataTable(UDataTable* DataTable);
	//BossAI 데이터 테이블 로드
	UFUNCTION(BlueprintCallable, Category = "Dialogue|BossAI")
	void LoadBossDialogueDataTable(UDataTable* DataTable);

	void LoadDataTables();
	void StopCurrentDialogue();

protected:
	//SupAI 대사 데이터
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue|SupportAI")
	TMap<EDialogueSupAI, FDialogueDataSupAI> DialogueSupAIMap;
	//BossAI 대사 데이터
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue|BossAI")
	TMap<EDialogueBossAI, FDialogueDataBossAI> DialogueBossAIMap;
	
	UPROPERTY()
	UAudioComponent* CurrentDialogue;

	//SupAI 시퀀스 관련 변수
	TArray<EDialogueSupAI> CurrentDialogueSequence;
	int32 CurrentSequenceIndex;
	FTimerHandle SequenceTimerHandle;

	//BossAI 시퀀스 관련 변수
	TArray<EDialogueBossAI> CurrentBossDialogueSequence;
	int32 CurrentBossSequenceIndex;
	FTimerHandle BossSequenceTimerHandle;
	
	TMap<EDialogueSupAI, float> LastPlayBackTimes;
	TMap<EDialogueBossAI, float> LastBossPlayBackTimes;

	EDialogueSupAI CurrentDialogueSupAIType;
	EDialogueBossAI CurrentBossDialogueType;

	FTimerHandle DialogueFinishTimerHandle;
	
	FString CurrentSubtitle;
	
	void PlayNextInSequence();
	void PlayNextBossInSequence();
	bool CanPlayDialogue(EDialogueSupAI DialogueType);
	bool CanPlayBossDialogue(EDialogueBossAI DialogueType);
	void HandleDialogueFinished();
};
