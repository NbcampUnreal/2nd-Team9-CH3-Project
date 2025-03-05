#include "2_AI/MeleeEnemyCharacter.h"
#include "2_AI/MeleeEnemyAIController.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 


AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	AIControllerClass = AMeleeEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // aicontroller 자동 빙의
	
	MaxHp = 100;
	CurrentHp = MaxHp;
	AttackPower = 10;
	bIsDead = false;
}

void AMeleeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//// NOTICE :: 5초 후에 Die() 호출 (테스트용)
	//GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AMeleeEnemyCharacter::Die, 5.0f, false);
}

TArray<AActor*> AMeleeEnemyCharacter::GetOverlappingPlayersForWeapon(FName WeaponBoneName)
{
    TArray<AActor*> OverlappingPlayers;

    USkeletalMeshComponent* MeshComp = GetMesh();
    if (!MeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Enemy] 적의 메쉬가 없슴다"));
        return OverlappingPlayers;
    }

    // 무기 충돌 바디 가져옴
    FBodyInstance* WeaponBody = MeshComp->GetBodyInstance(WeaponBoneName);
    if (!WeaponBody)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Enemy] 무기 바디 '%s'가 없슴다"), *WeaponBoneName.ToString());
        return OverlappingPlayers;
    }

    // 충돌바디 월드범위 가져옴
    FBox Bounds = WeaponBody->GetBodyBounds();

    // Overlap 검사(구 형태)
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 자기 자신은 제외

    bool bOverlap = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        Bounds.GetCenter(),
        FQuat::Identity,
        ECC_Pawn, // 플레이어가 Pawn 채널에 있을 경우
        FCollisionShape::MakeSphere(Bounds.GetExtent().GetMax()),
        QueryParams
    );

    if (bOverlap)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            if (AParagonAssetCharacter* Player = Cast<AParagonAssetCharacter>(Result.GetActor()))
            {
                OverlappingPlayers.AddUnique(Player);
            }
        }
    }

    return OverlappingPlayers;
}

void AMeleeEnemyCharacter::Attack()
{
    // 각 무기에 오버랩된 애들 가져옴
    TArray<AActor*> OverlappingPlayersR = GetOverlappingPlayersForWeapon(TEXT("weapon_r"));
    TArray<AActor*> OverlappingPlayersL = GetOverlappingPlayersForWeapon(TEXT("weapon_l"));

    // 배열 합쳐서 중복 제거
    TArray<AActor*> AllOverlappingPlayers = OverlappingPlayersR;
    for (AActor* Actor : OverlappingPlayersL)
    {
        AllOverlappingPlayers.AddUnique(Actor);
    }

    if (AllOverlappingPlayers.Num() > 0)
    {
        for (AActor* Actor : AllOverlappingPlayers)
        {
            if (AParagonAssetCharacter* Player = Cast<AParagonAssetCharacter>(Actor))
            {
                UGameplayStatics::ApplyDamage(Player, AttackPower, GetController(), this, UDamageType::StaticClass());
                UE_LOG(LogTemp, Log, TEXT("[Enemy] 플레이어 공격 적용"), *Player->GetName());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[Enemy] 플레이어에 아무것도 오버랩되지 않음"));
    }
}

float AMeleeEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead) return 0.0f;

    float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    CurrentHp -= damage;

    UE_LOG(LogTemp, Warning, TEXT("[Enemy] 적의 현재 체력 : %d"), CurrentHp);

    if (CurrentHp <= 0)
    {
        Die();
    }

    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BBComp = AICon->GetBlackboardComponent())
        {
            BBComp->SetValueAsBool(TEXT("IsHit"), true);
        }
    }

    return damage;
}

void AMeleeEnemyCharacter::ResetHitState()
{
    // 블랙보드의 IsHit 값을 false로 설정
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BBComp = AICon->GetBlackboardComponent())
        {
            BBComp->SetValueAsBool(TEXT("IsHit"), false);
        }
    }
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
    UE_LOG(LogTemp, Warning, TEXT("[Enemy] 적은 사망했습니다..."), CurrentHp);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ResetHitState();
	SetLifeSpan(4.0f);  // 자동으로 Destroy 호출
}
