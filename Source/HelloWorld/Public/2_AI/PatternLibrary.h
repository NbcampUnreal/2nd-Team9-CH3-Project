#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatternLibrary.generated.h"

class ABossCharacter;
class UThrowSwordSkill;
class USpawnMinionSkill;
class ULaserSkill;
class UPushAttackSkill;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELLOWORLD_API UPatternLibrary : public UActorComponent
{
	GENERATED_BODY()

public:	
	int32 GetRandomAttackIndex(const int GetMontageSize);

	UFUNCTION(BlueprintCallable, Category = "Boss|AttackSkill")
	void CallThrowSwordSkill(const FTransform& BossTransform, ABossCharacter* BossCharacter);

	UFUNCTION(BlueprintCallable, Category = "Boss|AttackSkill")
	void CallSpawnMinionSkill(const FTransform& BossTransform);

	UFUNCTION(BlueprintCallable, Category = "Boss|AttackSkill")
	void CallPushAttackSkill(const FTransform& BossTransform);
	
protected:
	TSubclassOf<UThrowSwordSkill> ThrowSwordSkillClass;
	TSubclassOf<USpawnMinionSkill> SpawnMinionSkillClass;
	TSubclassOf<UPushAttackSkill> PushAttackSkillClass;

	UThrowSwordSkill* ThrowSwordSkill;
	USpawnMinionSkill* SpawnMinionSkill;
	UPushAttackSkill* PushAttackSkill;

	virtual void BeginPlay() override;
};
