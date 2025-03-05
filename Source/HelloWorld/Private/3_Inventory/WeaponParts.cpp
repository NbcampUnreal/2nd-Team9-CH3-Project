// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/WeaponParts.h"

EPartsEffect UWeaponParts::GetPartsEffect() const
{
	return ItemData.PartsEffect;
}

EWeaponType UWeaponParts::GetWeaponType() const
{
	return ItemData.WeaponType;
}
