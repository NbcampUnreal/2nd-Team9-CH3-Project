#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ThrowSwordSkill.generated.h"

UCLASS()
class HELLOWORLD_API UThrowSwordSkill : public UObject
{
	GENERATED_BODY()
	
public:
	UThrowSwordSkill();
	
protected:
	void GetBossTransform();

	FVector BossLocation;
	FRotator BossRotation;
};
