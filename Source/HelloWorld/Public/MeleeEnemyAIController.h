#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeEnemyAIController.generated.h"

UCLASS()
class HELLOWORLD_API AMeleeEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMeleeEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;  // 빙의되는 순간에 호출되는 함수
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
//	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
//
//protected:
//	void PatrolRandomPos();

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* EnemyBehaviorTree;
	
};
