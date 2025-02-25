#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeEnemyCharacter.generated.h"

UCLASS()
class HELLOWORLD_API AMeleeEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMeleeEnemyCharacter();

protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxHp;
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 CurrentHp;
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 AttackPower;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeadMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	bool bIsDead;
	FTimerHandle DeathTimer;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	void Attack();
	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	void ApplyAttackDamage();
	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable, Category = "Enemy|Status")
	void Die();
	void DestroyEnemy();


};
