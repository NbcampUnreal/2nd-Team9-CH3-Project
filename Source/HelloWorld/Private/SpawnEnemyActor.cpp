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
	
}