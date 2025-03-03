#include "2_AI/PatternLibrary.h"
#include "2_AI/BossCharacter.h"
#include "2_AI/SpawnMinionSkill.h"
#include "2_AI/ThrowSwordSkill.h"

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
    if (!ThrowSwordSkill)
    {
        ThrowSwordSkill = NewObject<UThrowSwordSkill>(this, UThrowSwordSkill::StaticClass());
    }
}

void UPatternLibrary::CallSpawnMinionSkill(const FTransform& BossTransform)
{
    if (!SpawnMinionSkill) return;
    UE_LOG(LogTemp, Log, TEXT("[PatternLibrary] 미니언 스폰 스킬을 호출했습니다!"))
    // 스킬 호출 예시
    SpawnMinionSkill->SpawnMinion(BossTransform);
}

void UPatternLibrary::CallThrowSwordSkill(const FTransform& BossTransform, ABossCharacter* BossCharacter)
{
    if (!ThrowSwordSkill) return;

    ThrowSwordSkill->Attack(BossTransform, BossCharacter);
}
