// Fill out your copyright notice in the Description page of Project Settings.

#include "4_Character/ParagonAssetCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "0_Framework/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "1_UI/MyPlayerController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "1_UI/MyFunctionLibrary.h"
#include "1_UI/MyHUD.h"
#include "0_Framework/MyGameState.h"
#include "3_Inventory/InventoryManager.h"
#include "3_Inventory/ItemBase.h"
#include "3_Inventory/Weapon.h"
#include "3_Inventory/WeaponComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AParagonAssetCharacter

AParagonAssetCharacter::AParagonAssetCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	// bUseControllerRotationPitch = false;
	// bUseControllerRotationYaw = false;
	// bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HitScreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitScreen"));
	HitScreen->SetupAttachment(FollowCamera);

	AimScreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimScreen"));
	AimScreen->SetupAttachment(FollowCamera);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Weapon Component
	CurrentWeapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	CurrentWeapon->SetupAttachment(RootComponent);

	// Create Timeline instance
	CameraTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTimelineComponent"));
	HitScreenTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("HitScreenTimelineComponent"));

	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		TEXT("AIPerceptionStimuliSourceComponent"));

	FireState = EFireState::Waiting;
	ChargeState = EChargeState::Normal;
	HealthState = EHealthState::Healthy;
	ZoomState = EZoomState::NoZooming;

	ChargeTime = 1.0f;
	MaxHealth = 100;
	DangerHealth = 30;
	DashSpeed = 2500.0f;
	DashTime = 0.5f;
	WallKickSpeed = 1200.0f;
	DefaultWalkSpeed = 500.0f;
	SprintWalkSpeedMultiplier = 2.0f;

	bCanAirDash = true;
	bCanWallKick = false;
	bCanSpecialAction = true;
	
	Health = MaxHealth;
	CurrentTouchedWallNormal = FVector(0, 0, 0);

	// Configure character movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 1400.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 오를수 있는 바닥 각도 설정
	GetCharacterMovement()->SetWalkableFloorAngle(60.0f);
}

void AParagonAssetCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Make mesh be invisible after load
	HitScreen->SetScalarParameterValueOnMaterials("FadeAlpha", 0.0f);
	AimScreen->SetScalarParameterValueOnMaterials("FadeAlpha", 0.0f);

	if (CameraTimelineComponent && CameraZoomCurve)
	{
		CameraZoomHandler.BindUFunction(this, FName("CameraZoom"));
		CameraTimelineComponent->AddInterpFloat(CameraZoomCurve, CameraZoomHandler);
	}

	if (HitScreenTimelineComponent)
	{
		// Curve에셋 만들기 싫어서 하는 몸비틀기
		// UCurveFloat 동적 생성
		UCurveFloat* HitScreenLinearCurve = NewObject<UCurveFloat>();

		// FRichCurve 변수로 빼기
		FRichCurve* CurveData = &HitScreenLinearCurve->FloatCurve;

		// 키 추가 (보간 모드는 기본값으로 설정됨)
		FKeyHandle StartKeyHandle = CurveData->AddKey(0.0f, 1.0f);
		FKeyHandle EndKeyHandle = CurveData->AddKey(1.0f, 0.0f);

		// 키 보간 모드 설정 (Cubic 보간 적용; S커브)
		CurveData->SetKeyInterpMode(StartKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
		CurveData->SetKeyInterpMode(EndKeyHandle, ERichCurveInterpMode::RCIM_Cubic);

		// Timeline 설정
		HitScreenOpacityHandler.BindUFunction(this, FName("SetHitScreenOpacity"));
		HitScreenTimelineComponent->AddInterpFloat(HitScreenLinearCurve, HitScreenOpacityHandler);
	}
}

void AParagonAssetCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	UE_LOG(LogTemp, Log, TEXT("Landed"));
	bCanAirDash = true;
}

int32 AParagonAssetCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

int32 AParagonAssetCharacter::GetCurrentHealth() const
{
	return Health;
}

float AParagonAssetCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                         class AController* EventInstigator, AActor* DamageCauser)
{
	float OriginDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= OriginDamage;

	// 데미지 받으면 실제 로그에 추가!
	if (AMyGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr)
	{
		FString NewCombatLogMessage = FString::Printf(TEXT("%.1f의 피해를 받았습니다."), OriginDamage);
		GameState->AddCombatLogMessage(NewCombatLogMessage);
	}

	HitScreenTimelineComponent->PlayFromStart();

	if (Health > DangerHealth)
	{
		HealthState = EHealthState::Healthy;
	}
	else if (Health > 0)
	{
		HealthState = EHealthState::Danger;
	}
	else
	{
		if (HealthState != EHealthState::Dead)
		{
			HealthState = EHealthState::Dead;
			UE_LOG(LogTemp, Log, TEXT("You Die"));
			UMyFunctionLibrary::StartFadeOut(this);
		}
	}

	return OriginDamage;
}

FVector AParagonAssetCharacter::GetMuzzleLocation() const
{
	if (GetMesh()) // 캐릭터의 Skeletal Mesh가 존재하는지 확인
	{
		return GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
	}

	return FVector::ZeroVector; // 만약 소켓이 없으면 (0,0,0) 반환
}

FVector AParagonAssetCharacter::GetAimDirection() const
{
	FVector CameraLocation(0, 0, 0);
	FRotator CameraRotation(0, 0, 0);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	}

	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + CameraRotation.Vector() * 10'000.0f;

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

void AParagonAssetCharacter::EquipWeapon(FName WeaponID)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		// 인벤토리 조사
		if (UInventoryManager* IM = MyGameInstance->GetInventoryManager())
		{
			UE_LOG(LogTemp, Warning, TEXT("Check Inventory"))
			// ID로 Weapon 가져오기
			if (UItemBase* Item = IM->GetItemFromID(WeaponID))
			{
				UE_LOG(LogTemp, Warning, TEXT("Check Item"))
				if (Item->GetItemType() == EItemType::Weapon)
				{
					UWeapon* SelectedWeapon = Cast<UWeapon>(Item);
					TArray<UWeaponParts*> PartsArray = IM->GetWeaponParts(SelectedWeapon->GetItemName());
					CurrentWeapon->SetWeaponComponentData(SelectedWeapon, PartsArray);
					UE_LOG(LogTemp, Warning, TEXT("CHANGE WEAPON %s"), *SelectedWeapon->GetItemName().ToString());
					// 무기교체 몽타주 실행

					FireState = EFireState::Aiming;
					RunWeaponChangeAnim();
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

void AParagonAssetCharacter::Fire()
{
	RunFireAnim();
}

//////////////////////////////////////////////////////////////////////////
// Input Binding
void AParagonAssetCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// Jumping
			if (MyPlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->JumpAction, ETriggerEvent::Started, this,
				                                   &AParagonAssetCharacter::JumpStart);
				EnhancedInputComponent->BindAction(MyPlayerController->JumpAction, ETriggerEvent::Completed, this,
				                                   &AParagonAssetCharacter::JumpStop);
			}

			// Moving
			if (MyPlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->MoveAction, ETriggerEvent::Triggered, this,
				                                   &AParagonAssetCharacter::Move);
			}

			// Looking
			if (MyPlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->LookAction, ETriggerEvent::Triggered, this,
				                                   &AParagonAssetCharacter::Look);
			}
			// Weapon
			if (MyPlayerController->FireAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->FireAction, ETriggerEvent::Started, this,
				                                   &AParagonAssetCharacter::WeaponStart);
				EnhancedInputComponent->BindAction(MyPlayerController->FireAction, ETriggerEvent::Completed, this,
				                                   &AParagonAssetCharacter::WeaponStop);
			}

			// Zoom
			if (MyPlayerController->ZoomAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->ZoomAction, ETriggerEvent::Started, this,
				                                   &AParagonAssetCharacter::AimStart);
				EnhancedInputComponent->BindAction(MyPlayerController->ZoomAction, ETriggerEvent::Completed, this,
				                                   &AParagonAssetCharacter::AimStop);
			}

			// Dash
			if (MyPlayerController->DashAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->DashAction, ETriggerEvent::Started, this,
				                                   &AParagonAssetCharacter::Dash);
			}

			// WallKick
			if (MyPlayerController->WallKickAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->WallKickAction, ETriggerEvent::Started, this,
												   &AParagonAssetCharacter::WallKick);
			}

			// Crouch
			// if (MyPlayerController->CrouchAction)
			// {
			// 	EnhancedInputComponent->BindAction(MyPlayerController->CrouchAction, ETriggerEvent::Started, this,
			// 									   &AParagonAssetCharacter::CrouchStart);
			// 	EnhancedInputComponent->BindAction(MyPlayerController->CrouchAction, ETriggerEvent::Completed, this,
			// 									   &AParagonAssetCharacter::CrouchStop);
			// }

			// Sprint
			if (MyPlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->SprintAction, ETriggerEvent::Started, this,
												   &AParagonAssetCharacter::Sprint);
				EnhancedInputComponent->BindAction(MyPlayerController->SprintAction, ETriggerEvent::Completed, this,
												   &AParagonAssetCharacter::SprintStop);
			}
			
			// 여기서부터 UI 키 바인딩이요!!
			if (MyPlayerController->PauseMenuAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->PauseMenuAction, ETriggerEvent::Started,
				                                   MyPlayerController, &AMyPlayerController::ToggleGamePauseMenu);
			}

			if (MyPlayerController->InventoryAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->InventoryAction, ETriggerEvent::Started,
				                                   MyPlayerController, &AMyPlayerController::ToggleInventory);
			}

			if (MyPlayerController->MissionAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->MissionAction, ETriggerEvent::Started,
				                                   MyPlayerController, &AMyPlayerController::ToggleMission);
			}

			// 무기 교체
			if (MyPlayerController->WeaponSelectAction1)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->WeaponSelectAction1, ETriggerEvent::Started,
				                                   CurrentWeapon, &UWeaponComponent::SelectWeapon1);
			}

			if (MyPlayerController->WeaponSelectAction2)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->WeaponSelectAction2, ETriggerEvent::Started,
				                                   CurrentWeapon, &UWeaponComponent::SelectWeapon2);
			}
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Error,
			       TEXT(
				       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
			       ), *GetNameSafe(this));
		}
	}
}

// void AParagonAssetCharacter::SetMediumCharge()
// {
// 	ChargeState = EChargeState::Medium;
// 	GetWorldTimerManager().SetTimer(ChargeTimer, this, &AParagonAssetCharacter::SetFullCharge, ChargeTime, false);
// }
//
// void AParagonAssetCharacter::SetFullCharge()
// {
// 	ChargeState = EChargeState::Full;
// }

void AParagonAssetCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>().GetSafeNormal();
	if (MovementVector.IsNearlyZero()) return;
	
	bool bIsInAir = GetMovementComponent()->IsFalling();

	if (bIsInAir)
	{
		FVector Start = GetMesh()->GetComponentLocation();
		// FVector CurrentMoveDirection = GetMovementComponent()->Velocity.GetSafeNormal();
		FVector Forward = GetActorForwardVector() * MovementVector.X;
		FVector Right = GetActorRightVector() * MovementVector.Y;
		
		FVector End = Start + (Forward + Right) * 50;

		// Raycast to floor
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, Params);
		if (bHit)
		{
			bCanWallKick = true;
			CurrentTouchedWallNormal = HitResult.ImpactNormal;
			// UE_LOG(LogTemp, Warning, TEXT("Touch Wall"));
		}
		else
		{
			bCanWallKick = false;
			CurrentTouchedWallNormal = HitResult.ImpactNormal;
		}
		
		AddMovementInput(GetActorForwardVector(), MovementVector.X);
		AddMovementInput(GetActorRightVector(), MovementVector.Y);
	}
	else
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.X);
		AddMovementInput(GetActorRightVector(), MovementVector.Y);
	}
}

void AParagonAssetCharacter::JumpStart(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	Jump();
}

void AParagonAssetCharacter::JumpStop(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	StopJumping();
}

void AParagonAssetCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AParagonAssetCharacter::AimStart(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	if (!bCanSpecialAction) return;

	ZoomState = EZoomState::Zooming;
	FireState = EFireState::Aiming;

	ZoomStart();
}

void AParagonAssetCharacter::AimStop(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	if (!bCanSpecialAction) return;

	ZoomState = EZoomState::NoZooming;
	if (FireState != EFireState::Aiming) FireState = EFireState::Waiting;

	ZoomStop();
}

void AParagonAssetCharacter::WeaponStart(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	if (!bCanSpecialAction) return;

	FireState = EFireState::Aiming;

	CurrentWeapon->WeaponStart();
	// GetWorldTimerManager().SetTimer(ChargeTimer, this, &AParagonAssetCharacter::SetMediumCharge, ChargeTime, false);
}

void AParagonAssetCharacter::WeaponStop(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	if (!bCanSpecialAction) return;

	FireState = EFireState::AimingEnd;

	CurrentWeapon->WeaponEnd();
	// RunFireAnim();
	// GetWorldTimerManager().ClearTimer(ChargeTimer);

	// ChargeState = EChargeState::Normal;
}

void AParagonAssetCharacter::Dash(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	if (!bCanSpecialAction) return;

	if (auto bIsInAir = GetCharacterMovement()->IsFalling())
	{
		if (!bCanAirDash) return;
		UE_LOG(LogTemp, Log, TEXT("Dash"));

		bCanAirDash = false;

		RunDashAnim();

		FRotator ControllerRotation = GetControlRotation();
		FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);

		LaunchCharacter(ForwardDirection * DashSpeed, true, true);
	}
}

void AParagonAssetCharacter::WallKick(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	if (!bCanWallKick) return;
	if (!bCanSpecialAction) return;

	bCanAirDash = true;
	LaunchCharacter(CurrentTouchedWallNormal * WallKickSpeed, false, false);
}

// void AParagonAssetCharacter::CrouchStart(const FInputActionValue& Value)
// {
// 	if (!Controller) return;
// 	if (HealthState == EHealthState::Dead) return;
// 	if (!bCanSpecialAction) return;
//
// 	Crouch();
// }
//
// void AParagonAssetCharacter::CrouchStop(const FInputActionValue& Value)
// {
// 	if (!Controller) return;
// 	if (HealthState == EHealthState::Dead) return;
// 	if (!bCanSpecialAction) return;
// 	
// 	UnCrouch();
// }

void AParagonAssetCharacter::Sprint(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	bool bIsInAir = GetCharacterMovement()->IsFalling();
	if (bIsInAir) return;
	if (!bCanSpecialAction) return;
	
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * SprintWalkSpeedMultiplier;
}

void AParagonAssetCharacter::SprintStop(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	if (!bCanSpecialAction) return;

	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

FVector AParagonAssetCharacter::GetMuzzleLocation()
{
	if (GetMesh()) // 캐릭터의 Skeletal Mesh가 존재하는지 확인
	{
		return GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
	}

	return FVector::ZeroVector; // 만약 소켓이 없으면 (0,0,0) 반환
}

void AParagonAssetCharacter::ZoomStart()
{
	CameraTimelineComponent->Play();
	AimScreen->SetScalarParameterValueOnMaterials("FadeAlpha", 1.0f);
}

void AParagonAssetCharacter::ZoomStop()
{
	CameraTimelineComponent->Reverse();
	AimScreen->SetScalarParameterValueOnMaterials("FadeAlpha", 0.0f);
}

void AParagonAssetCharacter::CameraZoom(float Alpha)
{
	if (CameraBoom && CameraZoomCurve)
	{
		// 줌 거리 조정
		float MinZoom = 150.0f; // 가장 가까운 줌 값
		float MaxZoom = 300.0f; // 기본 줌 값
		float NewTargetArmLength = FMath::Lerp(MaxZoom, MinZoom, Alpha);
		CameraBoom->TargetArmLength = NewTargetArmLength;

		// X축 위치 조정 (카메라를 살짝 이동)
		float MinYOffset = 50.0f; // 줌 인 시 카메라가 살짝 옆으로 이동
		float MaxYOffset = 0.0f; // 기본 위치
		float NewYOffset = FMath::Lerp(MaxYOffset, MinYOffset, Alpha);
		CameraBoom->SocketOffset.Y = NewYOffset;
	}
}

void AParagonAssetCharacter::SetHitScreenOpacity(float Alpha)
{
	if (HitScreen)
	{
		HitScreen->SetScalarParameterValueOnMaterials(FName(TEXT("FadeAlpha")), Alpha);
	}
}

void AParagonAssetCharacter::OnFiringEnd()
{
	// 이 함수가 실행되기 직전에 Aim(마우스 왼쪽클릭)이 발생하면
	// AParagonAssetCharacter::Aim 가 실행 된 다음에
	// AParagonAssetCharacter::OnFiringEnd 이것도 실행되서
	// 조준중인데도 FireState가 Waiting이 되서 조준 모션이 안나와서
	// if문으로 감쌌음

	if (FireState != EFireState::Aiming && ZoomState == EZoomState::NoZooming)
	{
		FireState = EFireState::Waiting;
	}

	UE_LOG(LogTemp, Log, TEXT("FireEnd"));
}

void AParagonAssetCharacter::OnWeaponChangeEnd()
{
	if (ZoomState == EZoomState::NoZooming)
	{
		FireState = EFireState::Waiting;
	}

	UE_LOG(LogTemp, Log, TEXT("WeaponChangeEnd"));
}
