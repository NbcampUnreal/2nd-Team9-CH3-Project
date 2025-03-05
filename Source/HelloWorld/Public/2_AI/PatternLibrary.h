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
	UPatternLibrary();

	TSubclassOf<USpawnMinionSkill> SpawnMinionSkillClass;
	TSubclassOf<ULaserSkill> LaserSkillClass;
	TSubclassOf<UPushAttackSkill> PushAttackSkillClass;
	TSubclassOf<UThrowSwordSkill> ThrowSwordSkillClass;

	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void CallSpawnMinionSkill(const FTransform& BossTransform);
	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void CallThrowSwordSkill(const FTransform& BossTransform, ABossCharacter* BossCharacter);
	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void CallLaserSkill(const FTransform& BossTransform);
	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void CallPushAttackSkill(const FTransform& BossTransform);

	int32 GetRandomAttackIndex(const int GetMontageSize);
	
protected:
	virtual void BeginPlay() override;

	USpawnMinionSkill* SpawnMinionSkill;
	ULaserSkill* LaserSkill;
	UPushAttackSkill* PushAttackSkill;
	UThrowSwordSkill* ThrowSwordSkill;
};
