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
		BossBehaviorTree = AIBehavior.Object;
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();
    TargetRange = 1500.f;
}

void ABossAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    APawn* ControlledPawn = GetPawn();
    if (!PlayerPawn || !ControlledPawn) return;

    float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
    bool bIsInRange = (Distance <= TargetRange);

    if (GetBlackboardComponent())
    {
        GetBlackboardComponent()->SetValueAsBool(TEXT("IsTargetInRange"), bIsInRange);
        /*UE_LOG(LogTemp, Log, TEXT("Distance: %f, TargetRange: %f, bIsInRange: %s"),
            Distance, TargetRange, bIsInRange ? TEXT("true") : TEXT("false"));*/
    }

    if (LineOfSightTo(PlayerPawn))
    {
        SetFocus(PlayerPawn);
        if (GetBlackboardComponent())
        {
            GetBlackboardComponent()->SetValueAsBool(TEXT("IsTargetFind"), true);
        }
    }
    else
    {
        ClearFocus(EAIFocusPriority::Gameplay);
        if (GetBlackboardComponent())
        {
            GetBlackboardComponent()->SetValueAsBool(TEXT("IsTargetFind"), false);
        }
    }
}
