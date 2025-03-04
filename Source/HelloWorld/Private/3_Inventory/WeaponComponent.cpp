// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/WeaponComponent.h"

#include "3_Inventory/Bullet.h"
#include "3_Inventory/DevCharacter.h"
#include "3_Inventory/Item.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "3_Inventory/Weapon.h"
#include "3_Inventory/WeaponParts.h"
#include "Kismet/GameplayStatics.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Damage = 10;
	BonusDamage = 0;
	BonusSpeed = 0;
	WeaponType = EWeaponType::Riffle;
}

void UWeaponComponent::SetWeaponComponentData(UWeapon* Weapon, TArray<UWeaponParts*> PartsArray)
{
	Damage = Weapon->GetDamage();
	WeaponType = Weapon->GetWeaponType();
	BonusDamage = 0;
	BonusSpeed = 0;
	
	for (auto Parts : PartsArray)
	{
		if (Parts->GetPartsEffect() == EPartsEffect::DamageUp)
		{
			BonusDamage = 10;
		}
		if (Parts->GetPartsEffect() == EPartsEffect::SpeedUP)
		{
			BonusSpeed = 500;
		}
	}
}

void UWeaponComponent::WeaponStart()
{
	switch (WeaponType)
	{
	case EWeaponType::Riffle:
		ChargeAmount = 1.0f;
		FireBullet();
		GetWorld()->GetTimerManager().SetTimer(RiffleTimer, this, &UWeaponComponent::FireBullet, FireRate, true);
		break;
	case EWeaponType::Charging:
		bIsCharging = true;
		ChargeAmount = 1.0f;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("INVALID WEAPON TYPE"));
		break;
	}
}

void UWeaponComponent::WeaponEnd()
{
	switch (WeaponType)
	{
	case EWeaponType::Riffle:
		GetWorld()->GetTimerManager().ClearTimer(RiffleTimer);
		break;
	case EWeaponType::Charging:
		bIsCharging = false;
		FireBullet();
	default:
		break;
	}
}

void UWeaponComponent::FireBullet()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ADevCharacter* DevCharacter = Cast<ADevCharacter>(Character);
	if (ProjectileClass && DevCharacter)
	{
		FVector MuzzleLocation = DevCharacter->GetMuzzleLocation();
		FVector AimTarget = DevCharacter->GetAimDirection();

		FVector FireDirection = (AimTarget - MuzzleLocation).GetSafeNormal();
		FRotator MuzzleRotation = FireDirection.Rotation();
		
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation);
		// 총알 크기 초기화(충전형에만 크기 변화가 있음)
		Bullet->SetActorScale3D(Bullet->GetActorScale3D() * ChargeAmount);
		// 충돌체 반지름 초기화
		// Bullet->CollisionComponent->SetSphereRadius(10*ChargeAmount/2);
		// 총알 데미지 초기화
		Bullet->SetBulletDamage(Damage + BonusDamage);
		// 총알 스피드 초기화
		Bullet->SetBulletSpeed(1500 + BonusSpeed);
		// 총알 발사 애니메이션 실행
		//AParagonAssetCharacter::Fire();
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsCharging && ChargeAmount < MaxCharge)
	{
		ChargeAmount += DeltaTime*2;
	}
}
