// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/DevCharacter.h"
#include "3_Inventory/DevPlayerController.h"
#include "EnhancedInputComponent.h"
#include "3_Inventory/WeaponComponent.h"
#include "3_Inventory/InventoryManager.h"
#include "3_Inventory/ItemBase.h"
#include "0_Framework/MyGameInstance.h"
#include "3_Inventory/Weapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ADevCharacter::ADevCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
		
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;  
	SpringArmComp->bUsePawnControlRotation = true;  

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	CameraTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTimelineComponent"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CameraZoomCurveFinder
		(TEXT("/Game/_Dev/CT_CameraZoom.CT_CameraZoom"));
	if (CameraZoomCurveFinder.Succeeded())
	{
		CameraZoomCurve = CameraZoomCurveFinder.Object;
	}

	CurrentWeapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon")); // [승현] 추가 필요
	CurrentWeapon->SetupAttachment(RootComponent); // [승현] 추가 필요
}

void ADevCharacter::BeginPlay()
{
	Super::BeginPlay();
	CameraZoomHandler.BindUFunction(this, FName("CameraZoom"));
	CameraTimelineComponent->AddInterpFloat(CameraZoomCurve, CameraZoomHandler);
}


void ADevCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (ADevPlayerController* PlayerController = Cast<ADevPlayerController>(GetController()))
        {
            if (PlayerController->FireAction)
            {
                // IA_Fire 액션 키를 "한번 눌렀을 때" StartFire() 호출
                EnhancedInput->BindAction(
                    PlayerController->FireAction,
                    ETriggerEvent::Started,
                    this,
                    &ADevCharacter::StartFire
                );
				//IA_Fire 액션 키를 "뗐을 때" StopFire() 호출
            	EnhancedInput->BindAction(
            		PlayerController->FireAction,
            		ETriggerEvent::Completed,
            		this,
            		&ADevCharacter::StopFire
            	);
            }
        	
            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ADevCharacter::Look
                );
            }
        	
			if (PlayerController->ZoomAction)
			{
				// IA_Zoom 액션 마우스 버튼을 "누르고 있을 때" StartZoom() 호출 
				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Triggered,
					this,
					&ADevCharacter::StartZoom
				);
				// IA_Zoom 액션 마우스 버튼을 "뗐을 때" StopZoom() 호출
				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Completed,
					this,
					&ADevCharacter::StopZoom
				);
			}

        	if (PlayerController->ChooseWeapon1Action)
        	{
        		EnhancedInput->BindAction(
					PlayerController->ChooseWeapon1Action,
					ETriggerEvent::Started,
					this,
					&ADevCharacter::SelectWeapon1
				);
        	}
        	
        	if (PlayerController->ChooseWeapon2Action)
        	{
        		EnhancedInput->BindAction(
					PlayerController->ChooseWeapon2Action,
					ETriggerEvent::Started,
					this,
					&ADevCharacter::SelectWeapon2
				);
        	}
        	
        	if (PlayerController->ChooseWeapon3Action)
        	{
        		
        	}
        	
        }
    }
}

FVector ADevCharacter::GetAimDirection()
{
	FVector CameraLocation(0,0,0);
	FRotator CameraRotation(0,0,0);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	}

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + CameraRotation.Vector()* 10'000.0f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	// 캐릭터 충돌 제외
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		return HitResult.ImpactPoint;
	}
	// 충돌이 없을 시 최대 사거리 반환
	return TraceEnd;
}

void ADevCharacter::StartFire()
{
	CurrentWeapon->WeaponStart(); // [승현] 추가 필요
}

// 마우스 버튼을 떼면 총 발사 멈춤
void ADevCharacter::StopFire()
{
	CurrentWeapon->WeaponEnd(); // [승현] 추가 필요
}

void ADevCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

FVector ADevCharacter::GetMuzzleLocation()  // [승현] 추가 필요
{
	if (GetMesh())  // 캐릭터의 Skeletal Mesh가 존재하는지 확인
	{
		return GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
	}

	return FVector::ZeroVector;  // 만약 소켓이 없으면 (0,0,0) 반환
}

void ADevCharacter::StartZoom()
{
	CameraTimelineComponent->Play();
}

void ADevCharacter::StopZoom()
{
	CameraTimelineComponent->Reverse();
}

void ADevCharacter::CameraZoom(float Alpha)
{
	if (SpringArmComp && CameraZoomCurve)
	{
		// 줌 거리 조정
		float MinZoom = 150.0f; // 가장 가까운 줌 값
		float MaxZoom = 250.0f; // 기본 줌 값
		float NewTargetArmLength = FMath::Lerp(MaxZoom, MinZoom, Alpha);
		SpringArmComp->TargetArmLength = NewTargetArmLength;

		// X축 위치 조정 (카메라를 살짝 이동)
		float MinYOffset = 50.0f; // 줌 인 시 카메라가 살짝 옆으로 이동
		float MaxYOffset = 0.0f;   // 기본 위치
		float NewYOffset = FMath::Lerp(MaxYOffset, MinYOffset, Alpha);
		SpringArmComp->SocketOffset.Y = NewYOffset;
	}
}
//승현님 추가해주세요
void ADevCharacter::EquipWeapon(FName WeaponID)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		// 인벤토리 조사
		if (UInventoryManager* IM = MyGameInstance->GetInventoryManager())
		{
			// ID로 Weapon 가져오기
			if (const UItemBase* Item = IM->GetItemFromID(WeaponID))
			{
				if (Item->GetItemType() == EItemType::Weapon)
				{
					const UWeapon* SelectedWeapon = Cast<UWeapon>(Item);
					TArray<UWeaponParts*> PartsArray = IM->GetWeaponParts(SelectedWeapon->GetWeaponType());
					CurrentWeapon->SetWeaponComponentData(SelectedWeapon,PartsArray);
					UE_LOG(LogTemp, Warning, TEXT("CHANGE WEAPON %s"), *SelectedWeapon->GetItemName().ToString());
					TSoftObjectPtr<UMaterial> ItemMaterial = SelectedWeapon->GetWeaponMaterial();
					UMaterial* LoadedMaterial = ItemMaterial.LoadSynchronous();
					if (LoadedMaterial)
					{
						USkeletalMeshComponent* MeshComp = GetMesh();
						if (MeshComp)
						{
							MeshComp->SetMaterial(3, LoadedMaterial);
							
						}
					}
				}
			}
		}
	}	
}

void ADevCharacter::SelectWeapon1()
{
	EquipWeapon("Weapon_1");
}

void ADevCharacter::SelectWeapon2()
{
	EquipWeapon("Weapon_2");
}


