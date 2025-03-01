#include "BossCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/OverlapResult.h"


ABossCharacter::ABossCharacter()
{
	/*AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;*/  // aicontroller 자동 빙의
	bIsDead = false;
	MaxHp = 1000;
	CurrentHp = MaxHp;
	AttackPower = 20;
}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossCharacter::Attack()
{
	UE_LOG(LogTemp, Log, TEXT("보스는 %d의 공격력을 가졌다."), AttackPower);
}

float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.0f;
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHp -= damage;

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

	//AMeleeEnemyAIController* MeleeAIController = Cast<AMeleeEnemyAIController>(GetController());
	//if (MeleeAIController)
	//{
	//	MeleeAIController->StopMovement();
	//	MeleeAIController->UnPossess();  // AI 컨트롤러 해제
	//}
	UE_LOG(LogTemp, Warning, TEXT("[Boss] 보스몬스터는 사망했습니다..."), CurrentHp);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(4.0f);
}

void ABossCharacter::DestroyEnemy()
{
	Destroy();
}


