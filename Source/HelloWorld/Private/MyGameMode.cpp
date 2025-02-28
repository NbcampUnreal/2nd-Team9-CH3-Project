#include "MyGameMode.h"

#include "DevCharacter.h"
#include "MyGameState.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ADevCharacter::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();	//한글 깨지나요?
}