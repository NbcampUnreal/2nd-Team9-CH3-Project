#include "LaserSkill.h"
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
    //LaserEffect = nullptr;
}

void ULaserSkill::FireLaser(const FTransform& BossTransform)
{
    UWorld* World = GetWorldFromOuter();
    if (!World) return;

    FVector BossLocation = BossTransform.GetLocation();
    FRotator BossRotation = BossTransform.GetRotation().Rotator();

    // 레이저 시작점
    // (이 예시에서는 보스의 위치를 시작점으로 사용하지만,
    // 실제로는 보스의 양손 소켓 위치를 사용하면 더 자연스러울 수 있음)
    FVector LaserStart = BossLocation;
    FVector LaserEnd = LaserStart + BossRotation.Vector() * MaxLaserLength;

    // 라인 트레이스를 사용해 레이저 길이 조정 (벽 등 장애물 검사)
    FHitResult Hit;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(Cast<AActor>(GetOuter()));  // 보스 액터 무시

    bool bHit = World->LineTraceSingleByChannel(Hit, LaserStart, LaserEnd, ECC_Visibility, TraceParams);
    if (bHit)
    {
        LaserEnd = Hit.ImpactPoint;
    }

    // 디버그용 레이저 선 그리기 (LaserDuration 초 동안, 굵기 5)
    DrawDebugLine(World, LaserStart, LaserEnd, FColor::Red, false, LaserDuration, 0, 10.f);

    // 데미지 적용: 라인 트레이스 결과에 따라 데미지를 주는 예시
    if (bHit && Hit.GetActor())
    {
        float TotalDamage = LaserDamagePerSecond * LaserDuration;
        UGameplayStatics::ApplyDamage(Hit.GetActor(), TotalDamage, nullptr, nullptr, nullptr);
        UE_LOG(LogTemp, Log, TEXT("[BossLaserSkill] Applied %.2f damage to %s"), TotalDamage, *Hit.GetActor()->GetName());
    }

    // 레이저 이펙트 스폰 (옵션)
 /*   if (LaserEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(World, LaserEffect, FTransform(BossRotation, LaserStart));
    }*/
}

UWorld* ULaserSkill::GetWorldFromOuter() const
{
    UObject* MyOuter = GetOuter();
    if (!MyOuter)
    {
        return nullptr;
    }

    UActorComponent* Comp = Cast<UActorComponent>(MyOuter);
    if (Comp && Comp->GetOwner())
    {
        return Comp->GetOwner()->GetWorld();
    }

	return nullptr;
}
