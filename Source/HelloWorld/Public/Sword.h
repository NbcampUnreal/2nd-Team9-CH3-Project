#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

class ABossCharacter;

UCLASS()
class HELLOWORLD_API ASword : public AActor
{
	GENERATED_BODY()
	
public:
	ASword();
	
	// UFUNCTION(BlueprintCallable, Category = "Setup")
	// void SetBossCharacter(ACharacter* Boss);
	// UFUNCTION(BlueprintCallable, Category = "Setup")
	// void SetRelativePositionToBoss(float Distance, float Angle);
	// UFUNCTION(BlueprintCallable, Category = "Action")
	// void FireToTarget(FVector TargetLocation);
	
protected:
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sword")
	// float FlySpeed;
	// UPROPERTY()
	// ACharacter* BossCharacter;
	// UPROPERTY()
	// ACharacter* PlayerCharacter;

	virtual void BeginPlay() override;
	// virtual void Tick(float DeltaTime) override;

private:
	// FVector OriginLocation;
	// FVector TargetLocation;
	// float DistanceFromBoss;
	// float AngleAroundBoss;
	// bool bIsFired;
	//
	// void SetInAir();
};
