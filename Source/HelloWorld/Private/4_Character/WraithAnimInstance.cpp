// Fill out your copyright notice in the Description page of Project Settings.

#include "4_Character/WraithAnimInstance.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UWraithAnimInstance::UWraithAnimInstance()
{
	Speed = 0;
	YawDelta = 0.0f;
	LeanIntensity = 7.0f;
	RotationLastTick = {0.0f, 0.0f, 0.0f};
	bIsInAir = false;
	bIsAccelerating = false;
	FireState = EFireState::Waiting;
	ChargeState = EChargeState::Normal;
	HealthState = EHealthState::Healthy;
	// DashState = EDashState::Waiting;
}

void UWraithAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Owner = TryGetPawnOwner();
	if (!Owner) return;

	PlayerCharacter = Cast<AParagonAssetCharacter>(Owner);
	if (!PlayerCharacter) return;

	UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();

	bIsInAir = MovementComponent->IsFalling();

	Speed = PlayerCharacter->GetVelocity().Length();

	FRotator AimRotator = PlayerCharacter->GetBaseAimRotation() - PlayerCharacter->GetActorRotation();
	Pitch = AimRotator.Pitch;
	Yaw = AimRotator.Yaw;
	Roll = AimRotator.Roll;
	
	YawDelta = FMath::FInterpTo(
		YawDelta,
		// 그냥 빼주면 안됨
		// FMath::FindDeltaAngleDegrees(A, B): 방향을 고려한 A, B사이 최소 각도
		FMath::FindDeltaAngleDegrees(PlayerCharacter->GetActorRotation().Yaw, RotationLastTick.Yaw) / (DeltaSeconds * LeanIntensity), 
		DeltaSeconds,
		6.0f);

	RotationLastTick = PlayerCharacter->GetActorRotation();
	
	bIsAccelerating = MovementComponent->GetCurrentAcceleration().Length() > 0.0f;

	FireState = PlayerCharacter->GetFireState();
	ChargeState = PlayerCharacter->GetChargeState();
	HealthState = PlayerCharacter->GetHealthState();
	// DashState = PlayerCharacter->GetDashState();
}
