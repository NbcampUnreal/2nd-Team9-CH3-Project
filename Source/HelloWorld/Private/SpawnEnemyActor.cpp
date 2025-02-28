#include "SpawnEnemyActor.h"
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
	SpawnEnemy();
}

FVector ASpawnEnemyActor::GetRandomLocationToSpawn() const
{
	FVector BoxExtent = SpawnAreaBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnAreaBox->GetComponentLocation();
	
	return BoxOrigin + FVector(FMath::FRandRange(-BoxOrigin.X, BoxExtent.X), FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y), BoxOrigin.Z);
}

void ASpawnEnemyActor::SpawnEnemy()
{
	for (int32 i=0; i<MaxSpawnNumber; i++)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		EnemyClass,
		GetRandomLocationToSpawn(),
		FRotator::ZeroRotator);
	}
}