#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ThrowSwordSkill.generated.h"

class ABossCharacter;
class ASword;

UCLASS()
class HELLOWORLD_API UThrowSwordSkill : public UObject
{
	GENERATED_BODY()
	
public:
	UThrowSwordSkill();

	void Attack(const FTransform& BossTransform, ABossCharacter* BossCharacter);
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<ASword>> Swords;
	
private:
	int32 NumberOfSword;
	float DistanceFromBoss;
	float ZOffset;
	float FireSwordDuration;
	TSubclassOf<ASword> SwordClass;
	TArray<FTimerHandle> SwordFireTimer;
};
