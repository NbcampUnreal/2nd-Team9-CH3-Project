#include "1_UI/MyFunctionLibrary.h"
#include "1_UI/MyPlayerController.h"
#include "1_UI/ScreenEffectComponent.h"
#include "Kismet/GameplayStatics.h"
#include "0_Framework/MyGameState.h"

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
