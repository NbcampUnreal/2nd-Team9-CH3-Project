#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

UCLASS()
class HELLOWORLD_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMyGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "MyGameMode")
	void StartTutorial();
	UFUNCTION(BlueprintCallable, Category = "MyGameMode")
	void StartMainLobby();

private:
	FName CurrentLevelName;
};
