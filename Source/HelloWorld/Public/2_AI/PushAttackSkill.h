#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PushAttackSkill.generated.h"


UCLASS()
class HELLOWORLD_API UPushAttackSkill : public UObject
{
    GENERATED_BODY()

public:
    UPushAttackSkill();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|PushAttack")
    float TargetDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|PushAttack")
    float PushDamage;

    UFUNCTION(BlueprintCallable, Category = "Boss|PushAttack")
    void PushAttack(const FTransform& BossTransform);

protected:
    UWorld* GetWorldFromOuter() const;
};
