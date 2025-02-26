#include "MyGameMode.h"

#include "DevCharacter.h"
#include "MyGameState.h"
#include "MyPlayerController.h" // 영민

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ADevCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass(); // 영민
	GameStateClass = AMyGameState::StaticClass();
}