#include "Sword.h"

#include "Bullet.h"
#include "MyPlayerController.h"
#include "BossCharacter.h"
#include "ParagonAssetCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(SceneComponent);
	//벽이나 땅에 닿으면 멈추기 위한 SphereComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(SceneComponent);
	
	// 충돌 설정
	SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
	SphereComponent->SetNotifyRigidBodyCollision(true); // Hit 이벤트 활성화
	
	BossCharacter = nullptr;
	PlayerCharacter = nullptr;
	FlySpeed = 500.0f;
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
	bIsFired = true;
	
	if (BossCharacter)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void ASword::BeginPlay()
{
	Super::BeginPlay();

	// 충돌 이벤트 바인딩
	SphereComponent->OnComponentHit.AddDynamic(this, &ASword::OnSphereHit);
	
	//PlayerCharacter에 ParagonAssetCharacter로 캐스팅하여 저장
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (ACharacter* Character = MyPlayerController->GetCharacter())
			{
				PlayerCharacter = Cast<AParagonAssetCharacter>(Character);
			}
		}
	}
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation = GetActorLocation();
	if(!bIsFired)
	{
		RotateToPlayer();	//플레이어를 바라보도록 함
	}
	else
	{
		FireToTarget(TargetLocation, DeltaTime); //플레이어를 향해 검 발사
	}
}

void ASword::RotateToPlayer()
{
	if (!PlayerCharacter || !BossCharacter) return;
	
	// 검에서 플레이어로 향하는 방향 단위 벡터 계산
	FVector DirectionToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
	DirectionToPlayer.Normalize();
	// 검이 플레이어를 향하도록 회전
	FRotator TargetRotation = DirectionToPlayer.Rotation();
	// 검의 기본 방향이 위쪽(Z+)이므로, Pitch를 -90만큼 회전
	TargetRotation.Pitch -= 90.0f;
	// 계산된 회전값 결과
	FRotator NewRotation = FRotator(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll);
	// 회전 적용
	SetActorRotation(NewRotation);
}

// 충돌 이벤트 처리 함수
void ASword::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 플레이어, 보스, 불렛와의 충돌은 무시
	if (OtherActor == Cast<AActor>(PlayerCharacter) || OtherActor == Cast<AActor>(BossCharacter))
	{
		return;
	}
	// 불렛과의 충돌 무시 (클래스 타입으로 확인)
	if (OtherActor->IsA(ABullet::StaticClass()))
	{
		return;
	}
	
	StopSword();
}

// 검을 멈추는 함수
void ASword::StopSword()
{
	//틱 비활성화
	if (!PrimaryActorTick.bCanEverTick)
	{
		return;
	}
	SetActorTickEnabled(false);
}

void ASword::FireToTarget(FVector Target, float DeltaTime)
{
	if (!PlayerCharacter)
	{
		return;
	}
	else
	{
		TargetLocation = PlayerCharacter->GetActorLocation();
	}
	
	// 현재 위치에서 타겟 위치로 향하는 방향 벡터 계산
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	// 속도에 따라 이동할 거리 계산
	float DistanceToMove = FlySpeed * DeltaTime;
	// 새 위치 계산
	FVector NewLocation = CurrentLocation + Direction * DistanceToMove;
	
	// 위치 업데이트
	SetActorLocation(NewLocation);
	
	// 검이 날아가는 방향으로 회전
	FRotator NewRotation = Direction.Rotation();
	// 검의 끝이 진행 방향을 향하도록 90도 피치 조정
	NewRotation.Pitch -= 90.0f;
	SetActorRotation(NewRotation);
}
