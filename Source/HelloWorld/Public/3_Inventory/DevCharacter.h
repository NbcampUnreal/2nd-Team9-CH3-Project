// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "3_Inventory/Bullet.h"
#include "Components/TimelineComponent.h"
#include "DevCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class HELLOWORLD_API ADevCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADevCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABullet> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float FireRate = 0.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UTimelineComponent> CameraTimelineComponent;
    
	UPROPERTY(EditAnyWhere, Category = "Camera")
	TObjectPtr<class UCurveFloat> CameraZoomCurve;
    
	FOnTimelineFloat CameraZoomHandler;
	float DefaultSpringArmLength;
	float ZoomedSpringArmLength;

	FTimerHandle FireTimer;

	int32 CurrentWeaponNumber;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	FVector GetAimDirection();
	FVector GetMuzzleLocation();
	
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartFire();
	UFUNCTION()
	void StopFire();
	UFUNCTION()
	void StartZoom();
	UFUNCTION()
	void StopZoom();
	UFUNCTION()
	void SelectWeapon1();
	UFUNCTION()
	void SelectWeapon2();
	
	UFUNCTION()
	void CameraZoom(float Alpha);

	UFUNCTION()
	void EquipWeapon(UStaticMesh* NewMesh, UMaterialInterface* NewMaterial, FVector NewScale);
	
	void Fire();

};
