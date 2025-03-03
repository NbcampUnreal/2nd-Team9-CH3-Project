#pragma once

#include "GameFramework/CheatManager.h"
#include "CoreMinimal.h"
#include "HW_CheatManager.generated.h"

UCLASS()
class HELLOWORLD_API UHW_CheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UHW_CheatManager();
	~UHW_CheatManager();

	UFUNCTION(Exec, BlueprintCallable)
	void StartFadeOut();
};
