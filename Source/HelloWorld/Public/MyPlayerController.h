#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UScreenEffectComponent;
class UInputAction;

UCLASS()
class HELLOWORLD_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	UScreenEffectComponent* ScreenEfc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PauseMenuAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MissionAction;

	void ToggleGamePauseMenu();
	void ToggleInventory();
	void ToggleMission();

private:
	bool IsMissionOpen;
};
