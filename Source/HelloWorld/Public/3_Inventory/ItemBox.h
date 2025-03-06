// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

class UItemBase;
class UBoxComponent;

UCLASS()
class HELLOWORLD_API AItemBox : public AActor, public IInteractInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Static Mesh")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
	FName ItemID = "Empty";
	
public:	
	// Sets default values for this actor's properties
	AItemBox();
	UFUNCTION()
	virtual void OnInteract(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	void AccquireItem();
};
