#include "2_AI/LaserSkill.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


ULaserSkill::ULaserSkill()
{
	LaserDuration = 5.0f;
	MaxLaserLength = 1000.0f;
	LaserDamagePerSecond = 10.0f;
}

void ULaserSkill::FireLaser(const FTransform& BossTransform)
{
    UWorld* World = GetWorldFromOuter();
    if (!World) return;

    FVector BossLocation = BossTransform.GetLocation();
    FRotator BossRotation = BossTransform.GetRotation().Rotator();

    // 레이저 시작점
    FVector LaserStart = BossLocation;
    FVector LaserEnd = LaserStart + BossRotation.Vector() * MaxLaserLength;

    // 라인 트레이스로 장야물 판별
    FHitResult Hit;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(Cast<AActor>(GetOuter()));  // 보스 액터 무시

    bool bHit = World->LineTraceSingleByChannel(Hit, LaserStart, LaserEnd, ECC_Visibility, TraceParams);
    if (bHit)
    {
        LaserEnd = Hit.ImpactPoint;
    }

    // 디버그용 레이저 선임
    DrawDebugLine(World, LaserStart, LaserEnd, FColor::Red, false, LaserDuration, 0, 10.f);

    // 데미지 적용
    if (bHit && Hit.GetActor())
    {
        float TotalDamage = LaserDamagePerSecond * LaserDuration;
        UGameplayStatics::ApplyDamage(Hit.GetActor(), TotalDamage, nullptr, nullptr, nullptr);
        UE_LOG(LogTemp, Log, TEXT("[BossLaserSkill] Applied %.2f damage to %s"), TotalDamage, *Hit.GetActor()->GetName());
    }
}

UWorld* ULaserSkill::GetWorldFromOuter() const
{
    UObject* MyOuter = GetOuter();
    if (!MyOuter) return nullptr;

    UActorComponent* ActorComp = Cast<UActorComponent>(MyOuter);
    if (ActorComp && ActorComp->GetOwner())
        return ActorComp->GetOwner()->GetWorld();

    return nullptr;
}
