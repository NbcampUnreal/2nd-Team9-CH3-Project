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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Attack")
    float TargetDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Attack")
    float PushDamage;

    UFUNCTION(BlueprintCallable, Category = "Push Attack")
    void ExecutePushAttack(const FTransform& BossTransform);

protected:
    UWorld* GetWorldFromOuter() const;
};
