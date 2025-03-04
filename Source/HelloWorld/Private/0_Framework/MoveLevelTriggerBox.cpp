#include "0_Framework/MoveLevelTriggerBox.h"
#include "0_Framework/MyGameState.h"
#include "4_Character/ParagonAssetCharacter.h"

AMoveLevelTriggerBox::AMoveLevelTriggerBox()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMoveLevelTriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(AParagonAssetCharacter::StaticClass()))
	{
		if (AMyGameState* MyGameState = GetWorld()->GetGameState<AMyGameState>())
		{
			MyGameState->SetTargetLevelName(TargetLevelName);
			MyGameState->ShowJoinUI();
		}
	}
}