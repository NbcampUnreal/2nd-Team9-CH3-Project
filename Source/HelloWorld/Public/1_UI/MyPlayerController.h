#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UScreenEffectComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class HELLOWORLD_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	UScreenEffectComponent* ScreenEfc;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PauseMenuAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InventoryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MissionAction;

	void ToggleGamePauseMenu();
	void ToggleInventory();
	void ToggleMission();

protected:
	

	virtual void BeginPlay() override;

private:
	bool IsGamePauseMenuOpen;
	bool IsInventoryOpen;
	bool IsMissionOpen;
};
