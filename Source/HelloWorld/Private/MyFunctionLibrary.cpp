#include "MyFunctionLibrary.h"
#include "MyPlayerController.h"
#include "ScreenEffectComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameState.h"

float UMyFunctionLibrary::GetFadeDuration(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
		{
			if (UScreenEffectComponent* ScreenEffectComponent = MyPC->ScreenEfc)
			{
				return ScreenEffectComponent->GetFadeDuration();
			}
		}
	}
	return 0.0f;
}

void UMyFunctionLibrary::StartFadeOut(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
		{
			if (UScreenEffectComponent* ScreenEffect = MyPC->ScreenEfc)
			{
				ScreenEffect->StartFadeOut(3.0f);
			}
			MyPC->SetInputMode(FInputModeUIOnly());
		}
	}
}
