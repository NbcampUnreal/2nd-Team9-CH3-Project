#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScreenEffectComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOWORLD_API UScreenEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScreenEffectComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	TSubclassOf<UUserWidget> FadeInAndOutWidgetClass;
	
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeIn(float Duration);
	
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeOut(float Duration);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Fade")
	void FadeInAnimFunction(float Duration);

	float GetFadeDuration();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fade")
	UUserWidget* FadeInAndOutWidgetInstance;

private:
	float FadeDuration;
	
};
