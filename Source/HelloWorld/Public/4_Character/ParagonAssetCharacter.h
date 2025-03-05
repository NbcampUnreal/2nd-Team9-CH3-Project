// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/TimelineComponent.h"
#include "100_Util/Types.h"
#include "4_Character/IWeaponUser.h"
#include "ParagonAssetCharacter.generated.h"

class FOnTimelineFloat;
class USpringArmComponent;
class UCameraComponent;
class UAIPerceptionStimuliSourceComponent;
class UWeaponComponent;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class HELLOWORLD_API AParagonAssetCharacter : public ACharacter, public IWeaponUser
{
	GENERATED_BODY()

public:
	AParagonAssetCharacter();

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	UStaticMeshComponent* HitScreen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	UStaticMeshComponent* AimScreen;

	// AI Perception
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;
	
	// Camera Zoom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	TObjectPtr<class UTimelineComponent> CameraTimelineComponent;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Camera|Zoom")
	TObjectPtr<class UCurveFloat> CameraZoomCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	TObjectPtr<class UTimelineComponent> HitScreenTimelineComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UWeaponComponent* CurrentWeapon;
	
	FOnTimelineFloat CameraZoomHandler;
	FOnTimelineFloat HitScreenOpacityHandler;
	FOnTimelineFloat DashHandler;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	// float DefaultSpringArmLength;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	// float ZoomedSpringArmLength;
	
	UFUNCTION()
	void ZoomStart();
	UFUNCTION()
	void ZoomStop();
	UFUNCTION()
	void CameraZoom(float Alpha);
	UFUNCTION()
	void SetHitScreenOpacity(float Alpha);
	
	// State
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	EFireState FireState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	EChargeState ChargeState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	EHealthState HealthState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	EZoomState ZoomState;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	void OnFiringEnd();

	UFUNCTION(BlueprintCallable, Category = "State")
	void OnWeaponChangeEnd();
	
	// Time for Changing To Next Charge Level
	// FTimerHandle ChargeTimer;

	// Constants
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Health")
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Health")
	int32 DangerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Dash")
	float DashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Dash")
	float DashTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|WallKick")
	float WallKickSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|WalkSpeed")
	float DefaultWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|WalkSpeed")
	float SprintWalkSpeedMultiplier;
	
	// Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	int32 Health;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Charge")
	float ChargeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Charge")
	bool bCanAirDash;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WallKick")
	bool bCanWallKick;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WallKick")
	bool bCanSpecialAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WallKick")
	FVector CurrentTouchedWallNormal; 
	
	void SetMediumCharge();
	void SetFullCharge();

	/** Input Binding Functions */
	void Move(const FInputActionValue& Value);
	void JumpStart(const FInputActionValue& Value);
	void JumpStop(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void AimStart(const FInputActionValue& Value);
	void AimStop(const FInputActionValue& Value);
	void WeaponStart(const FInputActionValue& Value);
	void WeaponStop(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void WallKick(const FInputActionValue& Value);
	// void CrouchStart(const FInputActionValue& Value);
	// void CrouchStop(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void SprintStop(const FInputActionValue& Value);

	FVector GetMuzzleLocation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Anim|Weapon")
	void RunFireAnim();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim|Dash")
	void RunDashAnim();

	UFUNCTION(BlueprintImplementableEvent, Category = "Anim|Weapon")
	void RunWeaponChangeAnim();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay() override;

	virtual void Landed(const FHitResult& Hit) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	EFireState GetFireState() const { return FireState; };
	EChargeState GetChargeState() const { return ChargeState; };
	EHealthState GetHealthState() const { return HealthState; };
	void SetFireState(const EFireState NewFireState) { FireState = NewFireState; };
	void SetChargeState(const EChargeState NewChargeState) { ChargeState = NewChargeState; };
	void SetHealthState(const EHealthState NewHealthState) { HealthState = NewHealthState; };
	void SwitchCanSpecialAction() { bCanSpecialAction = !bCanSpecialAction; };
	int32 GetMaxHealth() const;
	int32 GetCurrentHealth() const;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	// IWeaponUser Methods
	virtual FVector GetMuzzleLocation() const override;
	virtual FVector GetAimDirection() const override;
	virtual void EquipWeapon(FName WeaponID) override;
	virtual void Fire() override;
};










