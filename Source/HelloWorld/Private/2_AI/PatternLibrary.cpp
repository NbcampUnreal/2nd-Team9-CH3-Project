#include "2_AI/PatternLibrary.h"
#include "2_AI/BossCharacter.h"
#include "2_AI/SpawnMinionSkill.h"
#include "2_AI/ThrowSwordSkill.h"
#include "2_AI/LaserSkill.h"
#include "2_AI/PushAttackSkill.h"


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
   if (!LaserSkill)  
   {  
       LaserSkill = NewObject<ULaserSkill>(this, ULaserSkill::StaticClass());  
   }  
   if (!ThrowSwordSkill)
   {
        ThrowSwordSkill = NewObject<UThrowSwordSkill>(this, UThrowSwordSkill::StaticClass());
    }

   if (!PushAttackSkill)  
   {  
       PushAttackSkill = NewObject<UPushAttackSkill>(this, UPushAttackSkill::StaticClass());  
   }  
}  

void UPatternLibrary::CallSpawnMinionSkill(const FTransform& BossTransform)  
{  
   if (!SpawnMinionSkill) return;  
   UE_LOG(LogTemp, Log, TEXT("[PatternLibrary] 미니언 스폰 스킬을 호출했습니다!"));  
   SpawnMinionSkill->SpawnMinion(BossTransform);  
}  

void UPatternLibrary::CallThrowSwordSkill(const FTransform& BossTransform, ABossCharacter* BossCharacter)
{
    if (!ThrowSwordSkill) return;

    ThrowSwordSkill->Attack(BossTransform, BossCharacter);
}

void UPatternLibrary::CallLaserSkill(const FTransform& BossTransform)  
{  
   if (!LaserSkill) return;  
   UE_LOG(LogTemp, Log, TEXT("[PatternLibrary] 레이저 스킬을 호출했습니다!"));  
   LaserSkill->FireLaser(BossTransform);  
}

void UPatternLibrary::CallPushAttackSkill(const FTransform& BossTransform)  
{  
   if (!PushAttackSkill) return;  
   UE_LOG(LogTemp, Log, TEXT("[PatternLibrary] 밀치기 스킬을 호출했습니다!"));  
   PushAttackSkill->ExecutePushAttack(BossTransform);

}
