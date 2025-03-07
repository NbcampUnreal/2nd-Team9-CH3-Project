// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeaponUser.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponUser : public UInterface
{
	GENERATED_BODY()
};

class HELLOWORLD_API IWeaponUser
{
	GENERATED_BODY()

public:
	virtual FVector GetMuzzleLocation() const = 0;
	virtual FVector GetAimDirection() const = 0;
	virtual void EquipWeapon(FName WeaponID) = 0;
	virtual void Fire() = 0;
	// virtual void SelectWeapon1() {};
	// virtual void SelectWeaopn2() {};
};


