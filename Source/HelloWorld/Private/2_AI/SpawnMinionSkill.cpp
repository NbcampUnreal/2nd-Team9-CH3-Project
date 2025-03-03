#include "2_AI/SpawnMinionSkill.h"
#include "2_AI/MeleeEnemyCharacter.h"

USpawnMinionSkill::USpawnMinionSkill()
{
	SpawnCount = 10;
	SpawnRadius = 500.0f;

    //static ConstructorHelpers::FClassFinder<AMeleeEnemyCharacter>EnemyBPClass(TEXT("/Game/_Blueprint/Enemy/BP_MeleeEnemyCharacter"));
    static ConstructorHelpers::FClassFinder<AMeleeEnemyCharacter>EnemyBPClass(TEXT("/Game/_Blueprint/Enemy/BP_BossMeleeEnemyCharacter"));

    if (EnemyBPClass.Succeeded())
    {
        MinionClass = EnemyBPClass.Class;
        UE_LOG(LogTemp, Log, TEXT("[SpawnMinionSkill] 스폰될 미니언을 성공적으로 가져옴"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[SpawnMinionSkill] 스폰될 미니언 주소를 찾을 수 없음"));
        MinionClass = nullptr;
    }
}

void USpawnMinionSkill::SpawnMinion(const FTransform& BossTransform)
{
	if (!MinionClass) return;

	UWorld* World = GetWorldFromOuter();
	if (!World) return;

    FVector BossLocation = BossTransform.GetLocation();
    FRotator BossRotation = BossTransform.GetRotation().Rotator();

    UE_LOG(LogTemp, Log, TEXT("[SpawnMinionSkill] 미니언이 소환됩니다"));

    // 간단히 원형으로 소환
    for (int32 i = 0; i < SpawnCount; i++)
    {
        float AngleDeg = 360.f / (float)SpawnCount * i;
        float RadAngle = FMath::DegreesToRadians(AngleDeg);

        FVector Offset(FMath::Cos(RadAngle), FMath::Sin(RadAngle), 0.f);
        Offset *= SpawnRadius;

        FVector SpawnLocation = BossLocation + Offset;
        FRotator SpawnRot = BossRotation; // or some logic
        FActorSpawnParameters SpawnParams;

        auto Spawned = World->SpawnActor<AMeleeEnemyCharacter>(
            MinionClass,
            SpawnLocation,
            SpawnRot,
            SpawnParams
        );

        if (Spawned)
        {
            UE_LOG(LogTemp, Log, TEXT("[SpawnMinionSkill] 스폰된 미니언: %s"), *Spawned->GetName());
        }
    }
	
}

UWorld* USpawnMinionSkill::GetWorldFromOuter() const
{
	UObject* MyOuter = GetOuter();
	if (!MyOuter) return nullptr;

	UActorComponent* ActorComp = Cast<UActorComponent>(MyOuter);
	if (ActorComp && ActorComp->GetOwner())
	{
		return ActorComp->GetOwner()->GetWorld();
	}

	return nullptr;
}
