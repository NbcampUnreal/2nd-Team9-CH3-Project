#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MoveLevelTriggerBox.generated.h"

class AMyPlayerController;
class AMyGameState;

UCLASS()
class HELLOWORLD_API AMoveLevelTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	AMoveLevelTriggerBox();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveLevelTrigger")
	FName TargetLevelName;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
