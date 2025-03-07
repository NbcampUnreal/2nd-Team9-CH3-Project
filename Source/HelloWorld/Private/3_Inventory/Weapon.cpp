// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/Weapon.h"


int32 UWeapon::GetDamage() const
{
	return ItemData.Damage;
}

EWeaponType UWeapon::GetWeaponType() const
{
	return  ItemData.WeaponType;
}

TSoftObjectPtr<UMaterial> UWeapon::GetWeaponMaterial() const
{
	return ItemData.ItemMaterial;
}




