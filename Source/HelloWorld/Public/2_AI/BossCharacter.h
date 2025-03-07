#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

enum class EDialogueBossAI : uint8;
class UPatternLibrary;
class UWidgetComponent;
class UCameraComponent;
class UDialogueSubsystem;

UCLASS()
class HELLOWORLD_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Status")
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attack")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attack")
	UPatternLibrary* PatternLibrary;

	UDialogueSubsystem* DialogueSubsystem;
	EDialogueBossAI LastPlayedDialogueBossAI;

	// MyHUD에서 보스 HP 업데이트 함수에서 필요해서 추가
	int32 GetMaxHp() const;
	int32 GetCurrentHp() const;

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

	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void GetRandomAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "Boss|Attack")
	void ExcutePushAttackSkill();

	UFUNCTION(BlueprintCallable, Category = "Boss|Status")
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintCallable, Category = "Boss|Status")
	void Die();
};
