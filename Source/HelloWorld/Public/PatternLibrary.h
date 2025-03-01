#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatternLibrary.generated.h"

class USpawnMinionSkill;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOWORLD_API UPatternLibrary : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPatternLibrary();

protected:
	virtual void BeginPlay() override;

	// 예시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attack")
	USpawnMinionSkill* SpawnMinionSkill;

	// 실질적으로 보스가 호출하는 함수
	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void ExecuteSpawnMinion();
};
