#include "MeleeEnemyAIController.h"
#include "MeleeEnemyCharacter.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMeleeEnemyAIController::MoveToCurrentPatrolPoint, 0.2f, false);
	}
}

void AMeleeEnemyAIController::MoveToCurrentPatrolPoint()
{
	float PatrolRadius = 2000.0f;
	AMeleeEnemyCharacter* EnemyCharacter = Cast<AMeleeEnemyCharacter>(GetPawn());
	
	if (!EnemyCharacter) return;
	else EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 700.0f;

	// TODO : SpawnVolum Class로 범위 지정할지, 트리거 볼륨으로 범위지정할지 정해서 범위에서 움직이게 구현
	// TODO : 캐릭터 추적되는지 테스트
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

	// TODO :: 얘 길가다가 길막하면 화나서 멈출 때, 다른 길 가게 수정
}
