#include "1_UI/MyFunctionLibrary.h"
#include "1_UI/MyPlayerController.h"
#include "1_UI/ScreenEffectComponent.h"
#include "Kismet/GameplayStatics.h"
#include "0_Framework/MyGameState.h"
#include "1_UI/MyHUD.h"

float UMyFunctionLibrary::GetFadeDuration(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UScreenEffectComponent* ScreenEffectComponent = MyPlayerController->ScreenEfc)
			{
				return ScreenEffectComponent->GetFadeDuration();
			}
		}
	}
	return 0.0f;
}

AMyHUD* UMyFunctionLibrary::GetMyHUD(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			return Cast<AMyHUD>(MyPlayerController->GetHUD());
		}
	}
	return nullptr;
}

void UMyFunctionLibrary::StartFadeOut(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UScreenEffectComponent* ScreenEffect = MyPlayerController->ScreenEfc)
			{
				ScreenEffect->StartFadeOut(3.0f);
			}
			MyPlayerController->SetInputMode(FInputModeUIOnly());
		}
	}
}
