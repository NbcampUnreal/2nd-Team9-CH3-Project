// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/TimelineComponent.h"
#include "Types.h"
#include "ParagonAssetCharacter.generated.h"

class FOnTimelineFloat;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAIPerceptionStimuliSourceComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class HELLOWORLD_API AParagonAssetCharacter : public ACharacter
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

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomAction;

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
	
	FOnTimelineFloat CameraZoomHandler;
	FOnTimelineFloat HitScreenOpacityHandler;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EFireState FireState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EChargeState ChargeState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EHealthState HealthState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EZoomState ZoomState;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	void OnFiringEnd();

	FTimerHandle ChargeTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 DangerHealth;

	// Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 Health;

	// Timer for Changing To Next Charge Level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Charge")
	float ChargeTime;

	void SetMediumCharge();
	void SetFullCharge();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void Jump() override;

	void StopJumping() override;

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void AimStart(const FInputActionValue& Value);

	void AimStop(const FInputActionValue& Value);

	void WeaponStart(const FInputActionValue& Value);

	void WeaponStop(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void Fire(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay() override;

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
	int32 GetMaxHealth() const;
	int32 GetCurrentHealth() const;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;
};



