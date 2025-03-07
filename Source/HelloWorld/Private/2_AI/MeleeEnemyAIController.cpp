#include "2_AI/MeleeEnemyAIController.h"
#include "2_AI/MeleeEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


AMeleeEnemyAIController::AMeleeEnemyAIController()
{
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
	}
}

void AMeleeEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AMeleeEnemyCharacter* MeleeEnemy = Cast<AMeleeEnemyCharacter>(GetPawn());
	if (!MeleeEnemy) return;
	
	if (bool IsPlayerInRange = GetBlackboardComponent()->GetValueAsBool(TEXT("IsPlayerInRange")))
	{
		SetFocus(PlayerPawn);
		MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}
