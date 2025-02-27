#include "MeleeEnemyCharacter.h"
#include "MeleeEnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "ParagonAssetCharacter.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	AIControllerClass = AMeleeEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // aicontroller 자동 빙의
	
	MaxHp = 100;
	CurrentHp = MaxHp;
	AttackPower = 20;
	bIsDead = false;
}

void AMeleeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// NOTICE :: 5초 후에 Die() 호출 (테스트용)
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AMeleeEnemyCharacter::Die, 5.0f, false);
}

void AMeleeEnemyCharacter::Attack()
{
	// 공격 범위 내 플레이어가 있으면 데미지 적용
	ApplyAttackDamage();
}

void AMeleeEnemyCharacter::ApplyAttackDamage()
{
	// TODO 2 :: 플레이어 완료되면 테스트해보기 
	// 오버랩으로 플레이어 찾음
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AParagonAssetCharacter::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		AParagonAssetCharacter* Player = Cast<AParagonAssetCharacter>(Actor);
		if (Player)
		{
			UGameplayStatics::ApplyDamage(Player, AttackPower, GetController(), this, UDamageType::StaticClass());
		}
	}
}


float AMeleeEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{  
   // TODO 1 :: 무기 완료되면 테스트  
   if (bIsDead) return 0.0f;  

   float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);  
   CurrentHp -= damage;  

   if (CurrentHp <= 0)  
   {  
       Die();  
   }  

   return damage;  
}

void AMeleeEnemyCharacter::Die()
{
	if (bIsDead) return;

	bIsDead = true;

	AMeleeEnemyAIController* MeleeAIController = Cast<AMeleeEnemyAIController>(GetController());
	if (MeleeAIController)
	{
		MeleeAIController->StopMovement();
		MeleeAIController->UnPossess();  // AI 컨트롤러 해제
		
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(4.0f);  // 자동으로 Destroy 호출
}

void AMeleeEnemyCharacter::DestroyEnemy()
{
	Destroy();
}

