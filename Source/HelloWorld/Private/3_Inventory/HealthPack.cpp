// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/HealthPack.h"

AHealthPack::AHealthPack()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void AHealthPack:: OnInteract(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	
}
