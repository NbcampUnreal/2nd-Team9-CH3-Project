#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

class USphereComponent;
class AParagonAssetCharacter;
class ABossCharacter;

UCLASS()
class HELLOWORLD_API ASword : public AActor
{
	GENERATED_BODY()
	
public:
	ASword();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="Components")
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USphereComponent* SphereComponent;
	ABossCharacter* BossCharacter;
	AParagonAssetCharacter* PlayerCharacter;
	
	UFUNCTION(BlueprintCallable, Category = "Sword")
	void FireSword();
	
	void SetBossCharacter(ABossCharacter* Boss);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sword")
	float FlySpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// 충돌 이벤트 처리 함수
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	// 검 멈추는 함수
	void StopSword();
	

private:
	
	FVector TargetLocation;
	FVector CurrentLocation;
	bool bIsFired;

	void RotateToPlayer();
	void FireToTarget(FVector Target, float DeltaTime);
};
