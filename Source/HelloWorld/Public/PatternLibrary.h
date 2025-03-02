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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attack")
	USpawnMinionSkill* SpawnMinionSkill;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attack")
	TSubclassOf<UThrowSwordSkill> ThrowSwordSkillClass;
	UPROPERTY()
	UThrowSwordSkill* ThrowSwordSkill;
	
	// 실질적으로 보스가 호출하는 함수
	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void CallSpawnMinionSkill(const FTransform& BossTransform);
	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void CallThrowSwordSkill(const FTransform& BossTransform, ABossCharacter* BossCharacter);
};
