// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class UItemBase;
class UWeaponParts;
enum class EWeaponType : uint8;
class UWeapon;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class HELLOWORLD_API UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	// 무기 데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|info")
	int Damage;
	// 무기 부착물 추가 데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|info")
	int BonusDamage;
	// 무기 부착물 추가 스피드
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|info")
	float BonusSpeed;
	
	// 무기 교체중 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|info")
	bool bIsChangingWeapon = false;
	
	// 무기 타입
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|info")
	EWeaponType WeaponType;

	// 연발용 타이머
	FTimerHandle RiffleTimer;

	// 단발 광클 막는 타이머
	FTimerHandle ClickEnableTimer;

	// 쿨타임 타이머
	FTimerHandle WeaponCooldownTimer;

	// 사용중인지
	bool bIsWeaponChanging = false;
	// 충전중인지(충전형 전용)
	bool bIsCharging = false;
	// 충전된 정도
	float ChargeAmount = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Charging")
	// 프레임당 충전량
	int32 ChargePerFrame = 2;
	// 최대 충전량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Charging")
	float MaxCharge = 30.0f;
	
	// 총알
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABullet> ProjectileClass;

	// 발사 타이머 간격(연발에서 사용)
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float FireRate = 0.3f;
	
	// 클릭 타이머 간격(연발에서 사용)
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float ClickRate = 0.3f;

	// 발사 소리
	UPROPERTY()
	USoundBase* GunFireSound;

	// 차징 소리
	UPROPERTY()
	USoundBase* GunChargingSound;
	
	UPROPERTY()
	UAudioComponent* ChargingAudio;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UWeaponComponent();

	EWeaponType GetWeaponType() const { return WeaponType; }

	// 총알 발사 가능 여부
	bool bCanFire = true;

	// 무기 데이터 설정 (무기 material 변경, 무기 데미지 설정, 무기 타입 설정, 부착물 조사 후 적용)
	void SetWeaponComponentData(const UWeapon* Weapon, TArray<UWeaponParts*> PartsArray);

	// 무기 클릭 시 동작
	void WeaponStart();

	// 무기 클릭 끝났을 때 동작
	void WeaponEnd();

	// 총알 발사(셍성)
	void FireBullet();

	// 파츠 장착
	void EquipParts(const UItemBase* PartInput);
	
	void SelectWeapon1();
	void SelectWeapon2();

	void SetWeaponChanging(bool Flag) { bIsWeaponChanging = Flag; }; 
};
