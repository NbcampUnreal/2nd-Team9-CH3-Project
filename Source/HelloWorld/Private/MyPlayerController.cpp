#include "MyPlayerController.h"
#include "ScreenEffectComponent.h"

AMyPlayerController::AMyPlayerController() 
{
	ScreenEfc = CreateDefaultSubobject<UScreenEffectComponent>(TEXT("ScreenEffect"));
}