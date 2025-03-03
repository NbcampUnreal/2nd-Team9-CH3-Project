#include "0_Framework/MyGameMode.h"

#include "0_Framework/MyGameState.h"
#include "1_UI/MyPlayerController.h" // 영민
#include "3_Inventory/DevCharacter.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ADevCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass(); // 영민
	GameStateClass = AMyGameState::StaticClass();
}
