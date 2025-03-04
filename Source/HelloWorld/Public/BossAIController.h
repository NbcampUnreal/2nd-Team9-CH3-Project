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

private:
	float LOSSeenTime = 0.0f;
	float LOSNotSeenTime = 0.0f;
	bool bIsTargetFound = false;

	// 시야가 지속적으로 확보되어야 하는 최소 시간 (예: 0.2초)
	const float LOSSeenThreshold = 0.2f;
	// 시야가 끊어진 상태로 유지되어야 하는 최소 시간 (예: 0.5초)
	const float LOSNotSeenThreshold = 0.5f;

};
