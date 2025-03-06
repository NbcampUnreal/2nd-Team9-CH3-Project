#include "2_AI/MeleeEnemyCharacter.h"
#include "2_AI/MeleeEnemyAIController.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "1_UI/MyFunctionLibrary.h"
#include "1_UI/MyHUD.h"
#include "0_Framework/MyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "3_Inventory/HealthPack.h"


AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
    OverheadWidget->SetupAttachment(GetMesh());
    OverheadWidget->SetWidgetSpace(EWidgetSpace::World);

	AIControllerClass = AMeleeEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	MaxHp = 100;
	CurrentHp = MaxHp;
	AttackPower = 10;
	bIsDead = false;
}

void AMeleeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

TArray<AActor*> AMeleeEnemyCharacter::GetOverlappingPlayersForWeapon(FName WeaponBoneName)
{
    TArray<AActor*> OverlappingPlayers;

    USkeletalMeshComponent* MeshComp = GetMesh();
    if (!MeshComp) return OverlappingPlayers;

    FBodyInstance* WeaponBody = MeshComp->GetBodyInstance(WeaponBoneName);
    if (!WeaponBody) return OverlappingPlayers;

    FBox Bounds = WeaponBody->GetBodyBounds();

    // Overlap 검사(구 형태)
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bOverlap = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        Bounds.GetCenter(),
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(Bounds.GetExtent().GetMax()),
        QueryParams
    );

    if (bOverlap)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            if (AParagonAssetCharacter* Player = Cast<AParagonAssetCharacter>(Result.GetActor()))
            {
                OverlappingPlayers.AddUnique(Player);
            }
        }
    }

    return OverlappingPlayers;
}

void AMeleeEnemyCharacter::Attack()
{
    // 각 무기에 오버랩된 애들 가져옴
    TArray<AActor*> OverlappingPlayersR = GetOverlappingPlayersForWeapon(TEXT("weapon_r"));
    TArray<AActor*> OverlappingPlayersL = GetOverlappingPlayersForWeapon(TEXT("weapon_l"));

    TArray<AActor*> AllOverlappingPlayers = OverlappingPlayersR;
    for (AActor* Actor : OverlappingPlayersL)
    {
        AllOverlappingPlayers.AddUnique(Actor);
    }

    if (AllOverlappingPlayers.Num() > 0)
    {
        for (AActor* Actor : AllOverlappingPlayers)
        {
            if (AParagonAssetCharacter* Player = Cast<AParagonAssetCharacter>(Actor))
            {
                UGameplayStatics::ApplyDamage(Player, AttackPower, GetController(), this, UDamageType::StaticClass());
            }
        }
    }
}

float AMeleeEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead) return 0.0f;

    float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    CurrentHp -= damage;

    AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this);
    if (HUD)
    {
        UpdateOverheadEnemyHP(damage); // 혹시 이거 안되면 그냥 Tick에다 넣어버리자.. 어쩔수 없다..
    }

    // 데미지 받으면 실제 로그에 추가!
    if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
    {
        FString NewCombatLogMessage = FString::Printf(TEXT("%.1f의 피해를 주었습니다."), damage);
        GameState->AddCombatLogMessage(NewCombatLogMessage);
    }
    
    if (CurrentHp <= 0)
    {
        Die();
    }

    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent())
        {
            BBComponent->SetValueAsBool(TEXT("IsHit"), true);
        }
    }

    return damage;
}

void AMeleeEnemyCharacter::ResetHitState()
{
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent())
        {
            BBComponent->SetValueAsBool(TEXT("IsHit"), false);
        }
    }
}

void AMeleeEnemyCharacter::Die()
{
	if (bIsDead) return;

	bIsDead = true;

    // 게임 스테이트에서 적이 죽으면 킬카운트 증가!
    if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
    {
        GameState->AddKillCount();

        // 데미지 받으면 실제 로그에 추가!
        int32 KillCount = GameState->GetKillCount();
        FString NewCombatLogMessage = FString::Printf(TEXT("적을 %d명 처치했습니다!!"), KillCount);
        GameState->AddCombatLogMessage(NewCombatLogMessage);
    }

    // 적의 오버헤드 위젯 인스턴스 가져와서 거기 킬마크를 또 가져와서 죽으면 띄워주기!
    UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
    if (!OverheadWidgetInstance) return;

    if (UFunction* ShowKillMarkAnimFunc = OverheadWidgetInstance->FindFunction(FName("ShowKillMarkFunction")))
    {
        OverheadWidgetInstance->ProcessEvent(ShowKillMarkAnimFunc, nullptr);
    }

	AMeleeEnemyAIController* MeleeAIController = Cast<AMeleeEnemyAIController>(GetController());
	if (MeleeAIController)
	{
		MeleeAIController->StopMovement();
		MeleeAIController->UnPossess();
	}

    if (FMath::FRand() < HeathPackDropChance)  // 10퍼 확률
    {
        AHealthPack* HealthPack = GetWorld()->SpawnActor<AHealthPack>();
        if (HealthPack)
        {
            HealthPack->SetDropLocation(this->GetActorLocation());
        }
    }

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ResetHitState();
	SetLifeSpan(4.0f);
}

void AMeleeEnemyCharacter::UpdateOverheadEnemyHP(float const damage)
{
    if (!OverheadWidget) return;

    // 1. 캐릭터 머리 위에 연결할 위젯의 실체 가져오기
    UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
    if (!OverheadWidgetInstance) return;

    if (UProgressBar* HPBar = Cast<UProgressBar>(OverheadWidgetInstance->GetWidgetFromName(TEXT("Enemy_HP_Bar"))))
    {
        // HPPercent = 0.0 ~ 1.0 범위의 값이 나오도록 설정
        const float HPPercent = (MaxHp > 0.f) ? (float)CurrentHp / (float)MaxHp : 0.f;

        // 프로그레스 바의 퍼센트 설정
        HPBar->SetPercent(HPPercent);

        // HP가 낮아지면 초록->노랑->주황->빨강 색으로 변하게!
        if (HPPercent > 0.7f)
        {
            HPBar->SetFillColorAndOpacity(FLinearColor::Green);
        }
        else if (HPPercent > 0.5f)
        {
            HPBar->SetFillColorAndOpacity(FLinearColor::Yellow);
        }
        else if (HPPercent > 0.3f)
        {
            HPBar->SetFillColorAndOpacity(FLinearColor(1.0f, 0.3f, 0.0f, 1.0f));  // 주황색
        }
        else
        {
            HPBar->SetFillColorAndOpacity(FLinearColor::Red);
        }
    }

    if (UTextBlock* TakeDamageText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("TakeDamage"))))
    {
        TakeDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), damage)));
        TakeDamageText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
    }

    // 데미지 받으면 표시된 데미지가 위로 사라지는 애니메이션
    if (UFunction* DamageFloatUpAnimFunc = OverheadWidgetInstance->FindFunction(FName("DamageFloatUpFunction")))
    {
        OverheadWidgetInstance->ProcessEvent(DamageFloatUpAnimFunc, nullptr);
    }
}

// 적의 체력바가 항상 카메라를 바라보게 회전시키는 함수
void AMeleeEnemyCharacter::UpdateEnemyHealthBarRotation()
{
    if (!OverheadWidget || !GetWorld()) return;

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FRotator NewRotation = FRotator(0, CameraRotation.Yaw + 180.0f, 0);

    // 기존 회전값과 비교해서 변할 때만 업데이트 (불필요한 연산 방지)
    if (!NewRotation.Equals(OverheadWidget->GetComponentRotation(), 0.1f))
    {
        OverheadWidget->SetWorldRotation(NewRotation);
    }
}

// Tick을 쓰지 않기 위해 노력했지만 다 실패했습니다..
void AMeleeEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateEnemyHealthBarRotation();
}
