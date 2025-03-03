#include "1_UI/ScreenEffectComponent.h"
#include "1_UI/MyPlayerController.h"
#include "Blueprint/UserWidget.h"

UScreenEffectComponent::UScreenEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	FadeInAndOutWidgetClass = nullptr;
	FadeInAndOutWidgetInstance = nullptr;
	FadeDuration = 0.0f;
}

void UScreenEffectComponent::StartFadeIn(float Duration)
{
	if (FadeInAndOutWidgetClass)
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			FadeInAndOutWidgetInstance = CreateWidget<UUserWidget>(MyPC, FadeInAndOutWidgetClass);

			if (FadeInAndOutWidgetInstance)
			{
				FadeInAndOutWidgetInstance->AddToViewport();

				MyPC->bShowMouseCursor = false;
				MyPC->SetInputMode(FInputModeGameOnly());
			}
		}

		if (UFunction* FadeInFunc = FadeInAndOutWidgetInstance->FindFunction(FName("FadeInAnimFunction")))
		{
			FadeDuration = Duration;
			FadeInAndOutWidgetInstance->ProcessEvent(FadeInFunc, nullptr);
		}
	}
}

void UScreenEffectComponent::StartFadeOut(float Duration)
{
	if (FadeInAndOutWidgetClass)
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			FadeInAndOutWidgetInstance = CreateWidget<UUserWidget>(MyPC, FadeInAndOutWidgetClass);

			if (FadeInAndOutWidgetInstance)
			{
				FadeInAndOutWidgetInstance->AddToViewport();

				MyPC->bShowMouseCursor = false;
				MyPC->SetInputMode(FInputModeGameOnly());
			}
		}

		if (UFunction* FadeOutFunc = FadeInAndOutWidgetInstance->FindFunction(FName("FadeOutAnimFunction")))
		{
			FadeDuration = Duration;
			FadeInAndOutWidgetInstance->ProcessEvent(FadeOutFunc, nullptr);
		}
	}
}

float UScreenEffectComponent::GetFadeDuration()
{
	return FadeDuration;
}

// 틱 안씁니다 << 주석 처리
/*void UScreenEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}*/
