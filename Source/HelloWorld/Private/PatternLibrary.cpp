#include "PatternLibrary.h"
#include "BossCharacter.h"
#include "SpawnMinionSkill.h"
#include "ThrowSwordSkill.h"

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
    if (!ThrowSwordSkillClass)
    {
        ThrowSwordSkill = NewObject<UThrowSwordSkill>(this, ThrowSwordSkillClass);
    }
}

void UPatternLibrary::CallSpawnMinionSkill(const FTransform& BossTransform)
{
    if (!SpawnMinionSkill) return;

    // 스킬 호출 예시
    //SpawnMinionSkill->Attack(BossTransform);
}

void UPatternLibrary::CallThrowSwordSkill(const FTransform& BossTransform, ABossCharacter* BossCharacter)
{
    if (!ThrowSwordSkill) return;

    ThrowSwordSkill->Attack(BossTransform, BossCharacter);
}
