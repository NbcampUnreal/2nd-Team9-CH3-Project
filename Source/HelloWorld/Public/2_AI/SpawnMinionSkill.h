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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion")
	TSubclassOf<AMeleeEnemyCharacter> MinionClass;  // 블루프린트에서 넣어줌

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion")
	int32 SpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion")
	float SpawnRadius;

	UFUNCTION(BlueprintCallable)
	void SpawnMinion(const FTransform& BossTransform);

protected:
	UWorld* GetWorldFromOuter() const;
};
