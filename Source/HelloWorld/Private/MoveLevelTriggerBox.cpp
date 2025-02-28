#include "MoveLevelTriggerBox.h"
#include "MyGameState.h"
#include "ParagonAssetCharacter.h"

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