#include "2_AI/Sword.h"

#include "3_Inventory/Bullet.h"
#include "1_UI/MyPlayerController.h"
#include "2_AI/BossCharacter.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneComponent);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(SceneComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//벽이나 땅에 닿으면 멈추기 위한 SphereComponent
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(SceneComponent);
	
	BossCharacter = nullptr;
	PlayerCharacter = nullptr;
	FlySpeed = 2000.0f;
	TargetLocation = FVector::ZeroVector;
	bIsFired = false;
}

//ThrowSwordSkill에서 호출하는 함수
void ASword::SetBossCharacter(ABossCharacter* Boss)
{
	BossCharacter = Boss;
}

//ThrowSwordSkill에서 호출하는 함수
void ASword::FireSword()
{
	if (bIsFired)
	{
		return;
	}
	bIsFired = true;

	if (PlayerCharacter)
	{
		FiringDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
	}
	FVector TargetDirection = TargetLocation - GetActorLocation();
	TargetDirection.Normalize();
	// 검이 플레이어를 향하도록 회전
	FRotator TargetRotation = TargetDirection.Rotation();
	// 검의 기본 방향이 위쪽(Z+)이므로, Pitch를 -90만큼 회전
	TargetRotation.Pitch += 90.0f;
	// 계산된 회전값 결과
	FRotator NewRotation = FRotator(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll);
	// 회전 적용
	SetActorRotation(NewRotation);
	// 보스로부터 분리 (만약 부착되어 있었다면)
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	
	// 초기 상태 설정
	bIsFired = false;
	// 플레이어 캐릭터 찾기
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<AParagonAssetCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		PlayerCharacterCapsuleComp = PlayerCharacter->GetCapsuleComponent();
	}
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation = GetActorLocation();
	// 발사되지 않은 상태라면 보스 주변에서 회전
	if (!bIsFired)
	{
		TargetLocation = PlayerCharacter->GetActorLocation();
		if (PlayerCharacterCapsuleComp)
		{
			TargetLocation.Z += PlayerCharacterCapsuleComp->GetScaledCapsuleHalfHeight()/2;
		}
		RotateToPlayer();
	}
	else
	{
		FireToTarget(DeltaTime);
	}
	//발사되었으면 Tick 비활성화됨
}

void ASword::RotateToPlayer()
{
	if (!PlayerCharacter || !BossCharacter) return;
	
	
}

// 검을 멈추는 함수
void ASword::StopSword()
{
	UE_LOG(LogTemp, Warning, TEXT("StopSword() Called!!!"));
	if (!bIsFired)
	{
		return;
	}
	// 발사 상태 해제
	bIsFired = false;
	
	SetActorTickEnabled(false);
	// 충돌 비활성화
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	
}

void ASword::FireToTarget(float DeltaTime)
{
	if (!PlayerCharacter || !bIsFired)
	{
		return;
	}
	
	FVector NewLocation = CurrentLocation + FiringDirection * FlySpeed * DeltaTime;
	SetActorLocation(NewLocation);
}