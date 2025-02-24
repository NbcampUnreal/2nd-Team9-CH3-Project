#include "MeleeEnemyAIController.h"
#include "MeleeEnemyCharacter.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AMeleeEnemyAIController::AMeleeEnemyAIController()
{
	// TODO :: BehaviorTree, BlackBoard 연결
	ConstructorHelpers::FObjectFinder<UBehaviorTree>AIBehavior(TEXT("BehaviorTree'/Game/_Blueprint/Enemy/AI/BT_MeleeEnemy.BT_MeleeEnemy'"));
	if (AIBehavior.Succeeded())
		EnemyBehaviorTree = AIBehavior.Object;
}

void AMeleeEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (EnemyBehaviorTree != nullptr)
	{
		RunBehaviorTree(EnemyBehaviorTree);

		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (PlayerPawn)
			{
				BlackboardComp->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
			}

			GetBlackboardComponent()->SetValueAsVector(TEXT("DefaultLocation"), GetPawn()->GetActorLocation());
		}
	}
}

void AMeleeEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// MEMO :: 코드에서 enemy에 접근하면 behaviorTree가 안되는데 무슨 이유인지...?
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);  // 플레이어 저장
	AMeleeEnemyCharacter* MeleeEnemy = Cast<AMeleeEnemyCharacter>(GetPawn());
	if (!MeleeEnemy) return;

	if (LineOfSightTo(PlayerPawn))  // 플레이어가 시야에 들어오면
	{
		SetFocus(PlayerPawn);  // 적 시선처리
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsTargetFindRange"), true);
		MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);  // 시선의 우선순위를 clear
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsTargetFindRange"), false);
		MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

//void AMeleeEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
//{
//	Super::OnMoveCompleted(RequestID, Result);
//	
//	if (Result.Code == EPathFollowingResult::Success)
//	{
//		FTimerHandle TimerHandle;
//		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMeleeEnemyAIController::PatrolRandomPos, 0.2f, false);
//	}
//}
//
//void AMeleeEnemyAIController::PatrolRandomPos()
//{
//	float PatrolRadius = 500.0f;
//	AMeleeEnemyCharacter* EnemyCharacter = Cast<AMeleeEnemyCharacter>(GetPawn());
//	
//	if (!EnemyCharacter) return;
//	else EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 120.0f;
//
//	// TODO : SpawnVolum Class로 범위 지정할지, 트리거 볼륨으로 범위지정할지 정해서 범위에서 움직이게 구현
//	// TODO : 캐릭터 추적되는지 테스트
//	FVector CurrentLocation = EnemyCharacter->GetActorLocation();
//	FNavLocation RandomNavLocation;
//	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
//	if (NavSys && NavSys->GetRandomPointInNavigableRadius(CurrentLocation, PatrolRadius, RandomNavLocation))
//	{
//		MoveToLocation(
//			RandomNavLocation.Location,
//			5.0f,
//			true,
//			true,
//			false,
//			true,
//			nullptr,
//			true
//		);
//	}
//}
