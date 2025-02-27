// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponParts.generated.h"

UCLASS()
class HELLOWORLD_API UWeaponParts : public UItemBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	EPartsEffect GetPartsEffect() const;
};
