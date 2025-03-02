#include "PatternLibrary.h"
#include "BossCharacter.h"
#include "SpawnMinionSkill.h"

UPatternLibrary::UPatternLibrary()
{
}


void UPatternLibrary::BeginPlay()
{
    Super::BeginPlay();

    // 공격 객체(UObject) 할당 예시
    if (!SpawnMinionSkill)
    {
        SpawnMinionSkill = NewObject<USpawnMinionSkill>(this, USpawnMinionSkill::StaticClass());
    }
}

void UPatternLibrary::CallSpawnMinionSkill(const FTransform& BossTransform)
{
    if (!SpawnMinionSkill) return;
    UE_LOG(LogTemp, Log, TEXT("[PatternLibrary] 미니언 스폰 스킬을 호출했습니다!"))
    // 스킬 호출 예시
    SpawnMinionSkill->SpawnMinion(BossTransform);
}
