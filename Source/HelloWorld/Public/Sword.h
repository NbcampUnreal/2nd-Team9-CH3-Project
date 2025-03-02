#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sword.generated.h"

class AParagonAssetCharacter;
class ABossCharacter;

UCLASS()
class HELLOWORLD_API ASword : public AActor
{
	GENERATED_BODY()
	
public:
	ASword();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	float FlySpeed;
	
	//추후 효과 추가 된다면?
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword|Effects")
	// USoundBase* StopSound;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword|Effects")
	// UParticleSystem* StopParticle;
	UFUNCTION(BlueprintCallable, Category = "Sword")
	void StopSword();
	
	void SetBossCharacter(ABossCharacter* Boss);
	void FireSword();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, Category = "Sword")
	FVector CurrentLocation;
	UPROPERTY(BlueprintReadWrite, Category = "Sword")
	FVector TargetLocation;
	UPROPERTY(BlueprintReadWrite, Category = "Sword")
	bool bIsFired;
	UPROPERTY(BlueprintReadWrite, Category = "Sword")
	ABossCharacter* BossCharacter;
	UPROPERTY(BlueprintReadWrite, Category = "Sword")
	AParagonAssetCharacter* PlayerCharacter;

	FVector FiringDirection;
	UCapsuleComponent* PlayerCharacterCapsuleComp;

	UFUNCTION(BlueprintCallable, Category = "Sword")
	void RotateToPlayer();
	UFUNCTION(BlueprintCallable, Category = "Sword")
	void FireToTarget(float DeltaTime);
};