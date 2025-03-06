#include "2_AI/BossCharacter.h"  
#include "2_AI/BossAIController.h"  
#include "2_AI/PatternLibrary.h"  
#include "4_Character/ParagonAssetCharacter.h"  
#include "1_UI/MyHUD.h"  
#include "1_UI/MyFunctionLibrary.h"  
#include "Components/CapsuleComponent.h"  
#include "Animation/AnimMontage.h"  
#include "Engine/OverlapResult.h"  
#include <Kismet/GameplayStatics.h>  
#include "Particles/ParticleSystem.h"  
#include "Particles/ParticleSystemComponent.h"  
#include "Camera/CameraComponent.h"  

ABossCharacter::ABossCharacter()  
{  
   AIControllerClass = ABossAIController::StaticClass();  
   AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  
   PatternLibrary = CreateDefaultSubobject<UPatternLibrary>(TEXT("PatternLibrary"));  

   bIsDead = false;  
   MaxHp = 4000;  
   CurrentHp = MaxHp;  
   AttackPower = 20;  
}  

int32 ABossCharacter::GetMaxHp() const  
{  
   return MaxHp;  
}  

int32 ABossCharacter::GetCurrentHp() const  
{  
   return CurrentHp;  
}  

void ABossCharacter::BeginPlay()  
{  
   Super::BeginPlay();
}  

void ABossCharacter::GetRandomAttackMontage()  
{  
   UE_LOG(LogTemp, Log, TEXT("보스는 %d의 공격력을 가졌다."), AttackPower);  

   if (!PatternLibrary) return;  

   FTransform BossTransform = GetActorTransform();  

   int32 RandomIndex = PatternLibrary->GetRandomAttackIndex(AttackMontages.Num());  
   if (RandomIndex == INDEX_NONE) return;  

   if (AttackMontages.Num() > RandomIndex && AttackMontages[RandomIndex])  
   {  
       PlayAnimMontage(AttackMontages[RandomIndex]);  
       UE_LOG(LogTemp, Log, TEXT("[BossCharacter] 공격몽타주배열의 %d번 몽타주 호출"), RandomIndex);  
   }  
}  

void ABossCharacter::ExcutePushAttackSkill()  
{  
   FTransform BossTransform = GetActorTransform();  
   PatternLibrary->CallPushAttackSkill(BossTransform);  
}  

float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)  
{  
   if (bIsDead) return 0.0f;  

   float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);  
   CurrentHp -= damage;  

   // 보스가 데미지를 받을 때 보스 HP 업데이트  
   if (AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this))  
   {  
       HUD->UpdateBossHPBar();  
   }  

   UE_LOG(LogTemp, Warning, TEXT("[Boss] 보스의 현재 체력 : %d"), CurrentHp);  

   if (CurrentHp <= 0)  
   {  
       Die();  
   }  

   return damage;  
}  

void ABossCharacter::Die()  
{  
   if (bIsDead) return;  

   bIsDead = true;  

   ABossAIController* BossAIController = Cast<ABossAIController>(GetController());  
   if (BossAIController)  
   {  
       BossAIController->StopMovement();  
       BossAIController->UnPossess();  
   }  
   UE_LOG(LogTemp, Warning, TEXT("[Boss] 보스 사망"), CurrentHp);  

   GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  
   SetLifeSpan(4.0f);  
}
