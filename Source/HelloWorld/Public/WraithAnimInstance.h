// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Types.h"
#include "WraithAnimInstance.generated.h"

class AParagonAssetCharacter;
/**
 * 
 */
UCLASS()
class HELLOWORLD_API UWraithAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UWraithAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	float Roll;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	float Yaw;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	float LeanIntensity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	FRotator RotationLastTick;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	bool bFullBody;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	AParagonAssetCharacter* PlayerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	EFireState FireState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
	EChargeState ChargeState;
};
