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

	TSubclassOf<AMeleeEnemyCharacter> MinionClass;  // 블루프린트에서 넣어줌

	int32 SpawnCount;

	float SpawnRadius;

	void SpawnMinion(const FTransform& BossTransform);

protected:
	UWorld* GetWorldFromOuter() const;
};
