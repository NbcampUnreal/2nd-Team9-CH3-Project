#include "ThrowSwordSkill.h"
#include "PatternLibrary.h"
#include "BossCharacter.h"
#include "Sword.h"

UThrowSwordSkill::UThrowSwordSkill()
{
	NumberOfSword = 4;
	ZOffset = 50.0f;
	DistanceFromBoss = 500.0f;
	FireSwordDuration = 1.0f; //검을 날리는 간격
	static ConstructorHelpers::FClassFinder<ASword> SwordBPClass(TEXT("/Game/_Blueprint/Boss/BP_Sword"));
	if (SwordBPClass.Succeeded())
	{
		SwordClass = SwordBPClass.Class;
	}
	else
	{
		//블루프린트 못 찾으면 C++클래스 기반 할당
		SwordClass = ASword::StaticClass();
	}
}

void UThrowSwordSkill::Attack(const FTransform& BossTransform, ABossCharacter* BossCharacter)
{
	if (!SwordClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ThrowSwordSkill: SwordClass가 유효하지 않습니다."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("ThrowSwordSkill: World를 가져올 수 없습니다."));
		return;
	}

	for (int32 i=0; i<NumberOfSword; i++)
	{
		//보스의 Yaw회전을 기준으로 머리 위에 원형으로 검 스폰하기 위한 Angle 계산
		float BossYaw = BossTransform.Rotator().Yaw;
		float Angle = FMath::DegreesToRadians( i * 360 / NumberOfSword + BossYaw );

		//상대적 위치 계산
		FVector RelativeLocation = FVector(
			FMath::Cos(Angle) * DistanceFromBoss,
			FMath::Sin(Angle) * DistanceFromBoss,
			ZOffset);
		//상대적 위치 계산을 바탕으로 검 스폰을 위한 Location 계산
		FVector WorldLocation = BossTransform.TransformPosition(RelativeLocation);
		FRotator SwordRotation = FRotator(0.0f, FMath::DegreesToRadians(Angle), 0.0f);
		//계산된 위치와 SpawnParams 기반으로 검 스폰
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ASword* NewSword = World->SpawnActor<ASword>(SwordClass, WorldLocation, SwordRotation, SpawnParams);

		if (NewSword)
		{
			Swords.Add(NewSword);
			NewSword->AttachToActor(Cast<AActor>(BossCharacter), FAttachmentTransformRules::KeepWorldTransform);
			NewSword->SetBossCharacter(BossCharacter);
		}
	}

	if (Swords.Num() == NumberOfSword)
	{
		for (int32 i=0; i<NumberOfSword; i++)
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([this, i]()
			{
				if (Swords.IsValidIndex(i) && Swords[i])
				{
					Swords[i]->FireSword();
				}
			});

			GetWorld()->GetTimerManager().SetTimer(SwordFireTimer, TimerDelegate, i * FireSwordDuration, false);
		}
	}
}
