#include "MeleeEnemyAIController.h"
#include "MeleeEnemyCharacter.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"

void AMeleeEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// TODO :: 초기화, 물리작업 추가
}

void AMeleeEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	MoveToCurrentPatrolPoint();
}

void AMeleeEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
		Super::OnMoveCompleted(RequestID, Result);
	
		if (Result.Code == EPathFollowingResult::Success)
		{
				MoveToCurrentPatrolPoint();
		}
}

void AMeleeEnemyAIController::MoveToCurrentPatrolPoint()
{
	// TODO :: PatrolPoints 를 타겟 포인트 말고 특정 범위 내에서 랜덤으로 넣는 부분 추가
	AMeleeEnemyCharacter* EnemyCharacter = Cast<AMeleeEnemyCharacter>(GetPawn());
	if (!EnemyCharacter) return;
	float PatrolRadius = 500.0f;

	FVector CurrentLocation = EnemyCharacter->GetActorLocation();
	FNavLocation RandomNavLocation;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys && NavSys->GetRandomPointInNavigableRadius(CurrentLocation, PatrolRadius, RandomNavLocation))
	{
		MoveToLocation(
			RandomNavLocation.Location,
			5.0f,
			true,
			true,
			false,
			true,
			nullptr,
			true
		);
	}
	// TODO :: TargetPos에 도착하면, 현재 위치 다시 CurrentPos에 저장하고 * offset 해주기
}
