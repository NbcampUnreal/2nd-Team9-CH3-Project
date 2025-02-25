#pragma once

#include "CoreMinimal.h"
#include "Types.generated.h"

UENUM(BlueprintType)
enum class EFireState : uint8
{
	Waiting, Aiming, Firing
};

UENUM(BlueprintType)
enum class EChargeState : uint8
{
	Normal, Medium, Full
};
