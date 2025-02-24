#include "MeleeEnemyCharacter.h"
#include "MeleeEnemyAIController.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	AIControllerClass = AMeleeEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // aicontroller 자동 빙의
}

void AMeleeEnemyCharacter::AttackMelee()
{
	// TODO :: 플레이어 발견 전까지는 MaxWalkSpeed = 300.0f, 발견 후에는 600.0f 추가


	// TODO :: 공격 구현

}





