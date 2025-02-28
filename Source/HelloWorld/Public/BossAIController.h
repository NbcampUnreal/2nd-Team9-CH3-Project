#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

UCLASS()
class HELLOWORLD_API ABossAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABossAIController();

	virtual void OnPossess(APawn* InPawn) override;  // 빙의되는 순간에 호출되는 함수
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* EnemyBehaviorTree;
};
