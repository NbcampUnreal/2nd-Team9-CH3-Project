#include "PatternLibrary.h"
#include "BossCharacter.h"
#include "SpawnMinionSkill.h"

UPatternLibrary::UPatternLibrary()
{
}


void UPatternLibrary::BeginPlay()
{
    Super::BeginPlay();

    // 예시코드임다 - 공격 객체들 생성 (UObject로 가정한다고 하네요 이부분은 지피티형님이 알려줌)
    if (!SpawnMinionSkill)
    {
        SpawnMinionSkill = NewObject<USpawnMinionSkill>(this, USpawnMinionSkill::StaticClass());
        //SpawnMinionSkill->InitializeAttack(this);
    }
}

void UPatternLibrary::ExecuteSpawnMinion()
{
    // 예시 코드
    if (SpawnMinionSkill)
    {
        ABossCharacter* BossOwner = Cast<ABossCharacter>(GetOwner());
        //SpawnMinionSkill->ExecuteAttack(BossOwner);
    }
}

