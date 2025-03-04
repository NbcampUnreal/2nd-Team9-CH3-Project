// Fill out your copyright notice in the Description page of Project Settings.


#include "3_Inventory/Bullet.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	
	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
		
	if(!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->InitSphereRadius(10.0f);
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		RootComponent = CollisionComponent;

		CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	}

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(CollisionComponent);

	if(!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 1500.0f;
		ProjectileMovementComponent->MaxSpeed = 1500.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

void ABullet::BeginPlay()
{
	InitialLifeSpan = 2.0f;
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet overlapped with: %s"), *OtherActor->GetName());
	}
	// 총알 제거
	Destroy();
}

void ABullet::SetBulletDamage(const int DamageInput)
{
	Damage = DamageInput;
}

void ABullet::SetBulletSpeed(const float SpeedInput)
{
	ProjectileMovementComponent->InitialSpeed = SpeedInput;
	ProjectileMovementComponent->MaxSpeed = SpeedInput;
}

