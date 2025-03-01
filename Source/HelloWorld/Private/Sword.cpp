
#include "Sword.h"

#include "Kismet/GameplayStatics.h"

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;
	// BossCharacter = nullptr;
	// FlySpeed = 500.0f;
}

//검을 던지는 클래스에서 호출해주면 됨
// void ASword::SetBossCharacter(ACharacter* Boss)
// {
// 	BossCharacter = Boss;
// }
//
// void ASword::SetRelativePositionToBoss(float Distance, float Angle)
// {
// 	DistanceFromBoss = Distance;
// 	AngleAroundBoss = Angle;
// }

void ASword::BeginPlay()
{
	Super::BeginPlay();

	// OriginLocation = GetActorLocation();
	// PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// void ASword::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// 	if(!bIsFired)
// 	{
// 		SetInAir();	//보스 주변을 돌면서 플레이어를 바라보도록 함
// 	}
// 	else
// 	{
// 		FireToTarget(TargetLocation); //플레이어를 향해 검 발사
// 	}
// }
//
// void ASword::SetInAir()
// {
// 	if(BossCharacter == nullptr || PlayerCharacter == nullptr) //보스나 플레이어가 없으면 종료
// 	{
// 		return;
// 	}
//
// 	// 보스 주변의 상대적 위치 계산 (극좌표계 -> 직교좌표계)
// 	FVector RelativeLocation = FVector(FMath::Cos(AngleAroundBoss) * DistanceFromBoss, FMath::Sin(AngleAroundBoss) * DistanceFromBoss, 0.0f);
// 	// 보스의 회전을 고려하여 상대적 위치 회전
// 	FVector RotatedRelativeLocation = BossCharacter->GetActorRotation().RotateVector(RelativeLocation);
// 	//최종 위치 계산 (보스 위치 + 회전된 상대적 위치)
// 	FVector NewLocation = BossCharacter->GetActorLocation() + RotatedRelativeLocation;
// 	SetActorLocation(NewLocation);
//
// 	//플레이어의 위치를 기반으로 플레이어를 바라보도록 검의 회전 설정
// 	FVector DirectionToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
// 	FRotator NewRotation = DirectionToPlayer.Rotation();
// 	SetActorRotation(NewRotation);
// }
//
// void ASword::FireToTarget(FVector TargetLocation)
// {
// 	
// }
