#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatternLibrary.generated.h"

class USpawnMinionSkill;
class ULaserSkill;
class UPushAttackSkill;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELLOWORLD_API UPatternLibrary : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPatternLibrary();

	TSubclassOf<USpawnMinionSkill> SpawnMinionSkillClass;
	TSubclassOf<ULaserSkill> LaserSkillClass;
	TSubclassOf<UPushAttackSkill> PushAttackSkillClass;

	void CallSpawnMinionSkill(const FTransform& BossTransform);
	void CallLaserSkill(const FTransform& BossTransform);
	void CallPushAttackSkill(const FTransform& BossTransform);

protected:
	virtual void BeginPlay() override;

	USpawnMinionSkill* SpawnMinionSkill;
	ULaserSkill* LaserSkill;
	UPushAttackSkill* PushAttackSkill;
};
