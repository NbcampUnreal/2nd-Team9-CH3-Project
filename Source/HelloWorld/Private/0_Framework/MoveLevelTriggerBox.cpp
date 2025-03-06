#include "0_Framework/MoveLevelTriggerBox.h"

#include "0_Framework/MyGameInstance.h"
#include "0_Framework/MyGameState.h"
#include "4_Character/ParagonAssetCharacter.h"

AMoveLevelTriggerBox::AMoveLevelTriggerBox()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMoveLevelTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		if (MyGameInstance && MyGameInstance->WasTriggerBoxUsed(TargetLevelName))
		{
			Destroy();
		}
	}
}

void AMoveLevelTriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(AParagonAssetCharacter::StaticClass()))
	{
		if (AMyGameState* MyGameState = GetWorld()->GetGameState<AMyGameState>())
		{
			MyGameState->SetUsedTriggerBox(TargetLevelName);
			MyGameState->SetTargetLevelName(TargetLevelName);
			MyGameState->ShowJoinUI();
		}
	}
}
