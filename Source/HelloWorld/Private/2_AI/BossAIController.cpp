#include "2_AI/BossAIController.h"
#include "2_AI/BossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


ABossAIController::ABossAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree>AIBehavior(TEXT("BehaviorTree'/Game/_Blueprint/Boss/AI/BT_Boss.BT_Boss'"));
	if (AIBehavior.Succeeded())
		EnemyBehaviorTree = AIBehavior.Object;
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (EnemyBehaviorTree != nullptr)
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABossAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);  // 플레이어 저장
	ABossCharacter* Boss = Cast<ABossCharacter>(GetPawn());
	if (!Boss) return;

	if (LineOfSightTo(PlayerPawn))  // 플레이어가 시야에 들어오면
	{
		SetFocus(PlayerPawn);  // 적 시선처리
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsTargetFind"), true);
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsTargetFind"), false);
	}
}
