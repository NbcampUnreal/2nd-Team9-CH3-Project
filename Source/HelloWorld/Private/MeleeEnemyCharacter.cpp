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

	// 체력이 0 이상이면 피격
	if (HitMontage && CurrentHp > 0)
	{
		PlayAnimMontage(HitMontage);
	}

	if (CurrentHp <= 0)
	{
		Die();
	}

	return damage;
}


void AMeleeEnemyCharacter::Die()
{
	// TODO 2: 사망 구현, 무기 완료되면 테스트
	bIsDead = true;

	AMeleeEnemyAIController* MeleeAIController = Cast<AMeleeEnemyAIController>(GetController());
	if (MeleeAIController)
	{
		MeleeAIController->StopMovement();
		MeleeAIController->UnPossess();  // ai 컨트롤러 해제
	}

	if (DeadMontage)
	{
		PlayAnimMontage(DeadMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(
		DeathTimer, this, &AMeleeEnemyCharacter::DestroyEnemy, 3.0f, false);
}

void AMeleeEnemyCharacter::DestroyEnemy()
{
	Destroy();
}








