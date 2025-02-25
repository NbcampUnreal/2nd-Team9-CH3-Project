﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Types.h"
#include "ParagonAssetCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EFireState FireState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EChargeState ChargeState;

	UFUNCTION(BlueprintCallable, Category = "State")
	void OnFiringEnd();

	FTimerHandle ChargeTimer;

	// Time for Changing To Next Charge Level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Charge")
	float ChargeTime; 
	
	void SetMediumCharge();
	void SetFullCharge();
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Aim(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void Fire(const FInputActionValue& Value);

	virtual void Fire_Implementation(const FInputActionValue& Value); // BlueprintNativeEvent's base function
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	EFireState GetFireState() const { return FireState; };
	EChargeState GetChargeState() const { return ChargeState; };
	void SetFireState(const EFireState NewFireState) { FireState = NewFireState; };
	void SetChargeState(const EChargeState NewChargeState) { ChargeState = NewChargeState; };
};


