#include "SpawnEnemyActor.h"
#include "NavigationSystem.h"
#include "Components/BoxComponent.h"

ASpawnEnemyActor::ASpawnEnemyActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneComponent);

	SpawnAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnAreaBox->SetupAttachment(SceneComponent);
	EnemyClass = nullptr;
}

void ASpawnEnemyActor::BeginPlay()
{
	Super::BeginPlay();
}

FVector ASpawnEnemyActor::GetRandomLocationToSpawn() const
{
	FVector BoxExtent = SpawnAreaBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnAreaBox->GetComponentLocation();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) 
	{
		UE_LOG(LogTemp, Warning, TEXT("네비게이션 시스템 못 찾음"));
		return BoxOrigin; // 네비게이션 시스템이 없으면 박스 중심 반환
	}
	
	// 네비게이션 메시 위의 랜덤 지점 찾아서 반환
	FNavLocation ResultLocation;
	float SearchRadius = FMath::Min(BoxExtent.X, BoxExtent.Y);
	bool bDirectSuccess = NavSystem->GetRandomReachablePointInRadius(
		BoxOrigin,
		SearchRadius,
		ResultLocation);
		
	if (bDirectSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("네비게이션 메시 위의 랜덤 지점 찾기 성공"));
		return ResultLocation.Location;
	}
	
	// 모든 방법이 실패하면 액터 위치 반환
	UE_LOG(LogTemp, Error, TEXT("모든 네비게이션 지점 찾기 실패, 액터 위치 사용"));
	return GetActorLocation();
}

void ASpawnEnemyActor::SpawnEnemy()
{
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: 적 클래스가 설정되지 않았습니다!"), *GetName());
		return;
	}

	int32 SuccessfulSpawns = 0;
	
	for (int32 i=0; i<MaxSpawnNumber; i++)
	{
		FVector SpawnLocation = GetRandomLocationToSpawn();
		
		// 유효한 위치인지 확인
		if (SpawnLocation.IsZero())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: 적 %d의 스폰 위치가 유효하지 않습니다"), *GetName(), i);
			continue;
		}
		
		// 스폰 파라미터 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		// 액터 스폰
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
			EnemyClass,
			SpawnLocation,
			FRotator::ZeroRotator,
			SpawnParams);
			
		if (SpawnedActor)
		{
			SuccessfulSpawns++;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: 적 %d 스폰 실패"), *GetName(), i);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s: %d개 중 %d개의 적 스폰 성공"), *GetName(), MaxSpawnNumber, SuccessfulSpawns);
}