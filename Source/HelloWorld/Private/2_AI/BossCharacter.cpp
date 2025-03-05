#include "2_AI/BossCharacter.h"
#include "2_AI/BossAIController.h"
#include "2_AI/PatternLibrary.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/OverlapResult.h"
#include <Kismet/GameplayStatics.h>


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

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*FTimerHandle TestTimer;
	GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &ABossCharacter::Attack, 4.0f, false);*/
}

void ABossCharacter::Attack()
{
	UE_LOG(LogTemp, Log, TEXT("보스는 %d의 공격력을 가졌다."), AttackPower);

	if (!PatternLibrary) return;

	FTransform BossTransform = GetActorTransform();

	int32 RandomIndex = PatternLibrary->GetRandomAttackIndex(AttackMontages.Num());
	if (RandomIndex == INDEX_NONE) return;
	
	if (AttackMontages.Num() > RandomIndex && AttackMontages[RandomIndex])
	{
		PlayAnimMontage(AttackMontages[RandomIndex]);
		UE_LOG(LogTemp, Log, TEXT("[BossCharacter] %d번 몽타주가 실행되는 중"), RandomIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몽타주 없어요 번호는 %d 번"), RandomIndex);
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
		BossAIController->UnPossess();  // AI 컨트롤러 해제
	}
	UE_LOG(LogTemp, Warning, TEXT("[Boss] 보스몬스터는 사망했습니다..."), CurrentHp);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(4.0f);
}



