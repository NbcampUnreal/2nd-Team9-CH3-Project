#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatternLibrary.generated.h"

class ABossCharacter;
class UThrowSwordSkill;
class USpawnMinionSkill;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOWORLD_API UPatternLibrary : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPatternLibrary();

	TSubclassOf<USpawnMinionSkill> SpawnMinionSkillClass;
	TSubclassOf<UThrowSwordSkill> ThrowSwordSkillClass;
	
	// 실질적으로 보스가 호출하는 함수
	void CallSpawnMinionSkill(const FTransform& BossTransform);
	void CallThrowSwordSkill(const FTransform& BossTransform, ABossCharacter* BossCharacter);

protected:
	virtual void BeginPlay() override;

	USpawnMinionSkill* SpawnMinionSkill;
	UThrowSwordSkill* ThrowSwordSkill;
};
