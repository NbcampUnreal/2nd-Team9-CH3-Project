// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "HealthPack.generated.h"

UCLASS()
class HELLOWORLD_API AHealthPack : public AActor, public IInteractInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Static Mesh")
	UStaticMeshComponent* StaticMesh;
	// 체력 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthPack")
	int32 HealingAmount;
	// Drop할 z축 위치 offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthPack")
	int32 DropPointOffset_Z;
	
public:	
	// Sets default values for this actor's properties
	AHealthPack();
	UFUNCTION(BlueprintCallable)
	virtual void OnInteract(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	
	void SetDropLocation(const FVector& EnemyLocation);
};
