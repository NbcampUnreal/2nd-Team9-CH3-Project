#include "5_Sound/DialogueSubsystem.h"

void UDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    
	// 초기화 코드
	UE_LOG(LogTemp, Log, TEXT("DialogueSubsystem initialized"));
}

void UDialogueSubsystem::Deinitialize()
{
	// 정리 코드
	UE_LOG(LogTemp, Log, TEXT("DialogueSubsystem deinitialized"));
    
	Super::Deinitialize();
}