#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnEnemyActor.generated.h"

class UBoxComponent;

UCLASS()
class HELLOWORLD_API ASpawnEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:
	ASpawnEnemyActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnEnemy")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnEnemy")
	UBoxComponent* SpawnAreaBox;	//스폰범위 지정할 박스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnEnemy")
	int32 MaxSpawnNumber;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnEnemy")
	TSubclassOf<APawn> EnemyClass;	//스폰 대상 클래스 지정(블루프린트에서)

	UFUNCTION(BlueprintCallable, Category = "SpawnEnemy")
	void SpawnEnemy();

protected:
	virtual void BeginPlay() override;

	FVector GetRandomLocationToSpawn() const;
};
