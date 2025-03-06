#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnMinionSkill.generated.h"

class AMeleeEnemyCharacter;

UCLASS()
class HELLOWORLD_API USpawnMinionSkill : public UObject
{
	GENERATED_BODY()
	
public:
	USpawnMinionSkill();

	TSubclassOf<AMeleeEnemyCharacter> MinionClass;
	int32 SpawnCount;
	float SpawnRadius;

	void SpawnMinion(const FTransform& BossTransform, UObject* WorldObject);

protected:
	UWorld* GetWorldFromOuter() const;
};
