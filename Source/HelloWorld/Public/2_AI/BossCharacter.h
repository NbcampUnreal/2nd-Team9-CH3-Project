#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

class UPatternLibrary;

UCLASS()
class HELLOWORLD_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Status")
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attack")
	UPatternLibrary* PatternLibrary;

protected:
	UPROPERTY(EditAnywhere, Category = "Boss|Combat")
	int32 MaxHp;

	UPROPERTY(EditAnywhere, Category = "Boss|Combat")
	int32 CurrentHp;

	UPROPERTY(EditAnywhere, Category = "Boss|Combat")
	int32 AttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Animation")
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Animation")
	UAnimMontage* AttackMontage;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Boss|Status")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Boss|Status")
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintCallable, Category = "Boss|Status")
	void Die();

	void DestroyEnemy();

};
