#include "2_AI/PushAttackSkill.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"


UPushAttackSkill::UPushAttackSkill()
{
    PushDamage = 5;
    TargetDistance = 2000.f;
}

void UPushAttackSkill::PushAttack(const FTransform& BossTransform, UObject* WorldObject)
{
    if (WorldObject == nullptr) return;

    AParagonAssetCharacter* PlayerCharacter = Cast<AParagonAssetCharacter>(UGameplayStatics::GetPlayerPawn(WorldObject, 0));
    if (!PlayerCharacter) return;

    // 보스와 플레이어의 수평 위치 계산 (Z = 0)
    FVector BossLocation = BossTransform.GetLocation();
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();

    FVector HorizontalBossLocation = BossLocation;
    HorizontalBossLocation.Z = 0;
    FVector HorizontalPlayerLocation = PlayerLocation;
    HorizontalPlayerLocation.Z = 0;

    // 보스에서 플레이어까지의 수평 방향 계산
    FVector Difference = HorizontalPlayerLocation - HorizontalBossLocation;
    FVector Direction = Difference.GetSafeNormal();
    if (Direction.IsNearlyZero())
    {
        // 보스랑 플레이어가 거의 같은 위치면 보스 전방 벡터 사용
        Direction = BossTransform.GetRotation().Rotator().Vector();
        Direction.Z = 0;
        Direction.Normalize();
    }

    // 보스 기준으로 TargetDistance 만큼 떨어진 위치 계산 (수평)
    FVector TargetPosition = HorizontalBossLocation + Direction * TargetDistance;

    // 필요한 수평 이동(Displacement)을 계산 (플레이어를 TargetPosition으로 보내기 위한)
    FVector Displacement = TargetPosition - HorizontalPlayerLocation;

    PlayerCharacter->LaunchCharacter(Displacement * 20.f, true, true);
    UGameplayStatics::ApplyDamage(PlayerCharacter, PushDamage, nullptr, Cast<AActor>(GetOuter()), nullptr);
    //DrawDebugDirectionalArrow(WorldObject->GetWorld(), HorizontalPlayerLocation, HorizontalPlayerLocation + Direction * 200.f, 50.f, FColor::Blue, false, 2.0f, 0, 5.f);
}
