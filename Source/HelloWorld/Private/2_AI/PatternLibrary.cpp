#include "2_AI/PatternLibrary.h"
#include "2_AI/BossCharacter.h"
#include "2_AI/SpawnMinionSkill.h"
#include "2_AI/ThrowSwordSkill.h"
#include "2_AI/LaserSkill.h"
#include "2_AI/PushAttackSkill.h"


void UPatternLibrary::BeginPlay()  
{  
   Super::BeginPlay();  

   if (!ThrowSwordSkill)
       ThrowSwordSkill = NewObject<UThrowSwordSkill>(this, UThrowSwordSkill::StaticClass());

   if (!SpawnMinionSkill)  
       SpawnMinionSkill = NewObject<USpawnMinionSkill>(this, USpawnMinionSkill::StaticClass());

   if (!PushAttackSkill)  
       PushAttackSkill = NewObject<UPushAttackSkill>(this, UPushAttackSkill::StaticClass());
}

int32 UPatternLibrary::GetRandomAttackIndex(const int GetMontageSize)
{
    if (GetMontageSize == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PatternLibrary] 몽타주 배열에 이상있음"));
        return INDEX_NONE;
    }

    return FMath::RandRange(0, GetMontageSize - 1);
}

void UPatternLibrary::CallThrowSwordSkill(const FTransform& BossTransform, ABossCharacter* BossCharacter)
{
    if (!ThrowSwordSkill) return;
    UE_LOG(LogTemp, Log, TEXT("[PatternLibrary] ThrowSword 호출"));
    ThrowSwordSkill->Attack(BossTransform, BossCharacter);
}

void UPatternLibrary::CallSpawnMinionSkill(const FTransform& BossTransform)  
{  
   if (!SpawnMinionSkill) return;  
   UE_LOG(LogTemp, Log, TEXT("[PatternLibrary] SpawnMinion 호출"));  
   SpawnMinionSkill->SpawnMinion(BossTransform);  
}

void UPatternLibrary::CallPushAttackSkill(const FTransform& BossTransform)  
{  
   if (!PushAttackSkill) return;  
   UE_LOG(LogTemp, Log, TEXT("[PatternLibrary] PushAttack 호출"));  
   PushAttackSkill->PushAttack(BossTransform);
}
