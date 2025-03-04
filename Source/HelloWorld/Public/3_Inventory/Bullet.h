// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class HELLOWORLD_API ABullet : public AActor
{
	GENERATED_BODY()

private:
	int Damage;
public:	
	ABullet();

	// 스피어 콜리전 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent* CollisionComponent;

	// 발사체 이동 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// 총알 메쉬 (3D 모델)
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BulletMesh;

	// 발사 방향으로의 발사체 속도를 초기화하는 함수
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION(BlueprintCallable, Category = Projectile)
	void OnHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse, const FHitResult& Hit);

	void SetBulletDamage(const int DamageInput);

	void SetBulletSpeed(const float SpeedInput);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
