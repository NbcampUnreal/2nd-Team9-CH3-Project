#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LaserSkill.generated.h"

class UParticleSystem;

UCLASS()
class HELLOWORLD_API ULaserSkill : public UObject
{
	GENERATED_BODY()
	
public:
	ULaserSkill();

	float LaserDuration;
	float MaxLaserLength;
	float LaserDamagePerSecond;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
	//UParticleSystem* LaserEffect;

	void FireLaser(const FTransform& BossTransform);

protected:
	UWorld* GetWorldFromOuter() const;

};
