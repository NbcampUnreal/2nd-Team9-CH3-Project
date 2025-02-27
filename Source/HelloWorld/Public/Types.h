#pragma once

#include "CoreMinimal.h"
#include "Types.generated.h"

UENUM(BlueprintType)
enum class EFireState : uint8
{
	Waiting, Aiming, AimingEnd
};

UENUM(BlueprintType)
enum class EChargeState : uint8
{
	Normal, Medium, Full
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	ChargeFire, RapidFire, Throw
};

UENUM(BlueprintType)
enum class EHealthState : uint8
{
	Healthy, Danger, Dead
};