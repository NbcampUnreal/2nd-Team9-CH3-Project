#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeEnemyCharacter.generated.h"

class UWidgetComponent;

UCLASS()
class HELLOWORLD_API AMeleeEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMeleeEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Status")
	bool bIsDead;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	void ResetHitState();

	void UpdateOverheadEnemyHP(float const damage);
	void UpdateEnemyHealthBarRotation();


protected:
	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|UI")
	UWidgetComponent* OverheadWidget;

	UPROPERTY(EditAnywhere, Category = "Enemy|Combat")
	int32 MaxHp;

	UPROPERTY(EditAnywhere, Category = "Enemy|Combat")
	int32 CurrentHp;

	UPROPERTY(EditAnywhere, Category = "Enemy|Combat")
	int32 AttackPower;

	// 헬스 팩 드롭 확률
	UPROPERTY(EditAnywhere, Category = "HealthPack")
	float HeathPackDropChance = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Animation")
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	TArray<AActor*> GetOverlappingPlayersForWeapon(FName WeaponBoneName);

	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	void Attack();
	
	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	void Die();

	void Tick(float DeltaTime);
	//void UpdateEnemyHealthBar();

};
