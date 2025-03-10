#include "2_AI/SpawnMinionSkill.h"
#include "2_AI/MeleeEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

USpawnMinionSkill::USpawnMinionSkill()
{
	SpawnCount = 10;
	SpawnRadius = 500.0f;

    static ConstructorHelpers::FClassFinder<AMeleeEnemyCharacter>EnemyBPClass(TEXT("/Game/_Blueprint/Enemy/BP_MeleeEnemyCharacter"));

    if (EnemyBPClass.Succeeded())
        MinionClass = EnemyBPClass.Class;
    else
        MinionClass = nullptr;
}

void USpawnMinionSkill::SpawnMinion(const FTransform& BossTransform, UObject* WorldObject)
{
	if (!MinionClass) return;
	
	//UWorld* World = WorldObject;
	
	if (WorldObject == nullptr) return;

    FVector BossLocation = BossTransform.GetLocation();
    FRotator BossRotation = BossTransform.GetRotation().Rotator();

    for (int32 i = 0; i < SpawnCount; i++)
    {
        float AngleDeg = 360.f / (float)SpawnCount * i;
        float RadAngle = FMath::DegreesToRadians(AngleDeg);

        FVector Offset(FMath::Cos(RadAngle), FMath::Sin(RadAngle), 0.f);
        Offset *= SpawnRadius;

        FVector SpawnLocation = BossLocation + Offset;
        FRotator SpawnRot = BossRotation;
        FActorSpawnParameters SpawnParams;

        auto Spawned = WorldObject->GetWorld()->SpawnActor<AMeleeEnemyCharacter>(
            MinionClass,
            SpawnLocation,
            SpawnRot,
            SpawnParams
        );
    }
	
}
