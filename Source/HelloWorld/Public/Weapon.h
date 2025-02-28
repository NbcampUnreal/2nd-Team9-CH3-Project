// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Weapon.generated.h"

UCLASS()
class HELLOWORLD_API UWeapon : public UItemBase
{
	GENERATED_BODY()

public:
	int32 GetDamage() const;
	EWeaponType GetWeaponType() const;
	TSoftObjectPtr<UMaterial> GetWeaponMaterial() const;
};
