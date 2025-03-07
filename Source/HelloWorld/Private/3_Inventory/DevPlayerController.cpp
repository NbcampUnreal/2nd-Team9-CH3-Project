// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/DevPlayerController.h"
#include "EnhancedInputSubsystems.h"

ADevPlayerController::ADevPlayerController()
	:InputMappingContext(nullptr),
	FireAction(nullptr),
	LookAction(nullptr),
	ZoomAction(nullptr)
{
	
}

void ADevPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

