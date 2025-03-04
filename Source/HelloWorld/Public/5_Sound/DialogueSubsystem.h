#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "5_Sound/SupAIDialogueTypes.h"
#include "DialogueSubsystem.generated.h"

UCLASS()
class HELLOWORLD_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//대사 재생
	UFUNCTION(BlueprintCallable, Category = "Dialogue|SupportAI")
	void PlaySupAIDialogue(EDialogueSupAI DialogueType);
	UFUNCTION(BlueprintCallable, Category = "Dialogue|SupportAI")
	void PlaySupAIDialogueSequence(const TArray<EDialogueSupAI>& DialogueSequence);
	//대사 데이터 설정
	UFUNCTION(BlueprintCallable, Category = "Dialogue|SupportAI")
	void SetDialogueData(EDialogueSupAI DialogueType, const FDialogueDataSupAI& DialogueData);
	//현재 재생 중인지 확인
	UFUNCTION(BlueprintPure, Category = "Dialogue|SupportAI")
	bool IsPlayingDialogue() const;
	//현재 자막 가져오기
	UFUNCTION(BlueprintPure, Category = "Dialogue|SupportAI")
	FString GetCurrentSubtitle() const;
	//대사 데이터맵 가져오기
	UFUNCTION(BlueprintPure, Category = "Dialogue|SupportAI")
	void SetDialogueDataMap(const TMap<EDialogueSupAI, FDialogueDataSupAI>& InDialogueMap);
	UFUNCTION(BlueprintCallable, Category = "Dialogue|SupportAI")
	void LoadDialogueDataTable(UDataTable* DataTable);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue|SupportAI")
	TMap<EDialogueSupAI, FDialogueDataSupAI> DialogueSupAIMap;
	UPROPERTY()
	UAudioComponent* CurrentDialogue;

	TArray<EDialogueSupAI> CurrentDialogueSequence;
	int32 CurrentSequenceIndex;
	FTimerHandle SequenceTimerHandle;
	
	TMap<EDialogueSupAI, float> LastPlayBackTimes;
	FString CurrentSubtitle;

	void PlayNextInSequence();
	bool CanPlayDialogue(EDialogueSupAI DialogueType);
};
