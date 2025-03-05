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

	void UpdateOverheadEnemyHP();
	void UpdateEnemyHealthBarRotation();


protected:
	// 타이머 핸들러
	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxHp;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 CurrentHp;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 AttackPower;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
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

	void DestroyEnemy();
	void Tick(float DeltaTime);
	void UpdateEnemyHealthBar();

};
