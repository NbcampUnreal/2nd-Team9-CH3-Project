// Fill out your copyright notice in the Description page of Project Settings.


#include "3_Inventory/Bullet.h"

#include "2_AI/MeleeEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "3_Inventory/WeaponComponent.h"
#include "4_Character/ParagonAssetCharacter.h"

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

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Game/_Sound/SFX/Gun_Hit.Gun_Hit"));
	if (tempSound.Succeeded())
	{
		BulletHitSound= tempSound.Object;
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

void ABullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 적이 오버랩되면 데미지 적용
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Enemy"))
		{
			UGameplayStatics::ApplyDamage(OtherActor, static_cast<float>(Damage),
				UGameplayStatics::GetPlayerController(this, 0),
				this, UDamageType::StaticClass()
				);
			UGameplayStatics::PlaySound2D(GetWorld(), BulletHitSound);

			// 총알 적 타격 이펙트 재생
			const FVector SurfacePoint = SweepResult.ImpactPoint + ((FVector)SweepResult.ImpactNormal * 3.0f);
			const FRotator SurfaceRotator = SweepResult.ImpactNormal.Rotation();
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				BulletHitEnemyEffect,
				SurfacePoint,
				SurfaceRotator,
				FVector(0.1f, 0.1f, 0.1f)
				);
			UE_LOG(LogTemp, Warning, TEXT("Bullet Hit Damage: %d"), Damage);

			// 차징형 무기가 아닐 때 총알 제거
			AParagonAssetCharacter* Player = Cast<AParagonAssetCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));			
			if (Player->GetCurrentWeapon()->GetWeaponType() != EWeaponType::Charging)
			{
				Destroy();	
			}
		}
	}
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// 총알 월드 타격 이펙트 재생
	const FVector SurfacePoint = Hit.ImpactPoint + ((FVector)Hit.ImpactNormal * 3.0f);
	const FRotator SurfaceRotator = Hit.ImpactNormal.Rotation();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		BulletHitWorldEffect,
		SurfacePoint,
		SurfaceRotator,
		FVector(0.1f, 0.1f, 0.1f)
		);
	// 총알 제거
	Destroy();
}

void ABullet::SetBulletDamage(const int DamageInput)
{
	Damage = DamageInput;
}

void ABullet::SetBulletSpeed(const float SpeedInput)
{
	ProjectileMovementComponent->MaxSpeed = ProjectileMovementComponent->InitialSpeed + SpeedInput;
	ProjectileMovementComponent->Velocity = ProjectileMovementComponent->Velocity.GetSafeNormal() * (ProjectileMovementComponent->MaxSpeed);
}

