// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/HealthPack.h"

#include "4_Character/ParagonAssetCharacter.h"

AHealthPack::AHealthPack()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!StaticMesh)
	{
		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
		ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
		if (SphereMesh.Succeeded())
		{
			StaticMesh->SetStaticMesh(SphereMesh.Object);
		}
		static ConstructorHelpers::FObjectFinder<UMaterial> HelthPackMaterial(TEXT("/Game/_Dev/juhun/M_HealthPack.M_HealthPack"));
		if (HelthPackMaterial.Succeeded())
		{
			StaticMesh->SetMaterial(0, HelthPackMaterial.Object);
		}
		StaticMesh->SetupAttachment(RootComponent);
		StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AHealthPack::OnInteract);
		StaticMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}
	

	HealingAmount = 20;
	DropPointOffset_Z = 10.0f;

	RootComponent->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
}

void AHealthPack:: OnInteract(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		AParagonAssetCharacter* Player = Cast<AParagonAssetCharacter>(OtherActor);
		// 승현님 SetCurrentHealth 함수 추가해주세요
		// Player->SetCurrentHealth(Player->GetCurrentHealth() + HealingAmount);
		UE_LOG(LogTemp, Display, TEXT("Healing Player %d"), HealingAmount);
		Destroy();
	}
}

// 다혜님 Enemy 죽는 곳에 아래 코드 추가해주세요
// Enemy가 죽을 때 해당 위치에 HealthPack 생성
// AHealthPack* HealthPack = GetWorld()->SpawnActor<AHealthPack>();
// if(HealthPack)
// {
// 	HealthPack->SetDropLocation(this->GetActorLocation);
// }
void AHealthPack::SetDropLocation(const FVector& EnemyLocation)
{
	this->SetActorLocation(EnemyLocation + FVector3d::UnitZ() * DropPointOffset_Z);
}

