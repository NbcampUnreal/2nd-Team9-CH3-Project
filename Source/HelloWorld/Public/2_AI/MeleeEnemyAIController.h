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

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* EnemyBehaviorTree;
	
};
