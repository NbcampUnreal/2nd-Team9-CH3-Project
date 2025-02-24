#include "MeleeEnemyCharacter.h"
#include "MeleeEnemyAIController.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	AIControllerClass = AMeleeEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // aicontroller 자동 빙의
}





