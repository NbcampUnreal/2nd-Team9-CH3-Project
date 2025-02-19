#include "MyGameMode.h"
#include "MyGameState.h"

AMyGameMode::AMyGameMode()
{
	//DefaultPawnClass = ? ;
	//PlayerControllerClass = ? ;
	GameStateClass = AMyGameState::StaticClass();	//한글 깨지나요?
}