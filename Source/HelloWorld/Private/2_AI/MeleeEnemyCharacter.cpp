#include "2_AI/MeleeEnemyCharacter.h"
#include "2_AI/MeleeEnemyAIController.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "1_UI/MyFunctionLibrary.h"
#include "1_UI/MyHUD.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
    OverheadWidget->SetupAttachment(GetMesh());
    OverheadWidget->SetWidgetSpace(EWidgetSpace::World);

	AIControllerClass = AMeleeEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // aicontroller 자동 빙의
	
	MaxHp = 1000;
	CurrentHp = MaxHp;
	AttackPower = 10;
	bIsDead = false;
}

void AMeleeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//// NOTICE :: 5초 후에 Die() 호출 (테스트용)
	//GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AMeleeEnemyCharacter::Die, 5.0f, false);
}

TArray<AActor*> AMeleeEnemyCharacter::GetOverlappingPlayersForWeapon(FName WeaponBoneName)
{
    TArray<AActor*> OverlappingPlayers;

    USkeletalMeshComponent* MeshComp = GetMesh();
    if (!MeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Enemy] 적의 메쉬가 없슴다"));
        return OverlappingPlayers;
    }

    // 무기 충돌 바디 가져옴
    FBodyInstance* WeaponBody = MeshComp->GetBodyInstance(WeaponBoneName);
    if (!WeaponBody)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Enemy] 무기 바디 '%s'가 없슴다"), *WeaponBoneName.ToString());
        return OverlappingPlayers;
    }

    // 충돌바디 월드범위 가져옴
    FBox Bounds = WeaponBody->GetBodyBounds();

    // Overlap 검사(구 형태)
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 자기 자신은 제외

    bool bOverlap = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        Bounds.GetCenter(),
        FQuat::Identity,
        ECC_Pawn, // 플레이어가 Pawn 채널에 있을 경우
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

    // 배열 합쳐서 중복 제거
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
                UE_LOG(LogTemp, Log, TEXT("[Enemy] 플레이어 공격 적용"), *Player->GetName());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[Enemy] 플레이어에 아무것도 오버랩되지 않음"));
    }
}

float AMeleeEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead) return 0.0f;

    float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    CurrentHp -= damage;

    UE_LOG(LogTemp, Warning, TEXT("[Enemy] 적의 현재 체력 : %d"), CurrentHp);

    AMyHUD* HUD = UMyFunctionLibrary::GetMyHUD(this);
    if (HUD)
    {
        UpdateOverheadEnemyHP(damage); // 혹시 이거 안되면 그냥 Tick에다 넣어버리자.. 어쩔수 없다..
    }
    
    if (CurrentHp <= 0)
    {
        Die();
    }

    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BBComp = AICon->GetBlackboardComponent())
        {
            BBComp->SetValueAsBool(TEXT("IsHit"), true);
        }
    }

    return damage;
}

void AMeleeEnemyCharacter::ResetHitState()
{
    // 블랙보드의 IsHit 값을 false로 설정
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BBComp = AICon->GetBlackboardComponent())
        {
            BBComp->SetValueAsBool(TEXT("IsHit"), false);
        }
    }
}

void AMeleeEnemyCharacter::Die()
{
	if (bIsDead) return;

	bIsDead = true;

	AMeleeEnemyAIController* MeleeAIController = Cast<AMeleeEnemyAIController>(GetController());
	if (MeleeAIController)
	{
		MeleeAIController->StopMovement();
		MeleeAIController->UnPossess();  // AI 컨트롤러 해제
	}
    UE_LOG(LogTemp, Warning, TEXT("[Enemy] 적은 사망했습니다..."), CurrentHp);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ResetHitState();
	SetLifeSpan(4.0f);  // 자동으로 Destroy 호출
}

void AMeleeEnemyCharacter::DestroyEnemy()
{
	Destroy();
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

        // HP가 낮을 때 빨갛게 변경
        if (HPPercent > 0.5f)
        {
            HPBar->SetFillColorAndOpacity(FLinearColor::Green);
        }
        else if (HPPercent > 0.3f)
        {
            HPBar->SetFillColorAndOpacity(FLinearColor::Yellow);
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
