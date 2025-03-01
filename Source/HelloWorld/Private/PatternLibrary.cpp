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

void UPatternLibrary::CallSpawnMinionSkill(FTransform BossTransform)
{
    if (!SpawnMinionSkill) return;

    // 스킬 호출 예시
    //SpawnMinionSkill->Attack(BossTransform);
}
