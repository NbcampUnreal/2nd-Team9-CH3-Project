#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UScreenEffectComponent;

UCLASS()
class HELLOWORLD_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	UScreenEffectComponent* ScreenEfc;
};
