#include "ParagonAssetCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "MyFunctionLibrary.h"

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

	// Configure character movement
	// GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 오를수 있는 바닥 각도 설정
	GetCharacterMovement()->SetWalkableFloorAngle(60.0f);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Create Timeline instance
	CameraTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTimelineComponent"));
	
	FireState = EFireState::Waiting;
	ChargeState = EChargeState::Normal;
	HealthState = EHealthState::Healthy;
	ZoomState = EZoomState::NoZooming;
	
	ChargeTime = 1.0f;
	MaxHealth = 100;
	DangerHealth = 30;
	
	Health = MaxHealth;
}

void AParagonAssetCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (CameraTimelineComponent && CameraZoomCurve)
	{
		CameraZoomHandler.BindUFunction(this, FName("CameraZoom"));
		CameraTimelineComponent->AddInterpFloat(CameraZoomCurve, CameraZoomHandler);
	}

	// Suicide
	// GetWorldTimerManager().SetTimer(
	// 	SuicideTimer,
	// 	TFunction<void()>([this]()
	// 	{
	// 		UGameplayStatics::ApplyDamage(this, 20.0f, nullptr, nullptr, UDamageType::StaticClass());
	// 	}),
	// 	1.0f,
	// 	true);
}

float AParagonAssetCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float OriginDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= OriginDamage;

	UE_LOG(LogTemp, Log, TEXT("kwaark! damage: %f"), OriginDamage);

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

//////////////////////////////////////////////////////////////////////////
// Input
void AParagonAssetCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
		{
			// Jumping
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AParagonAssetCharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AParagonAssetCharacter::StopJumping);

			// Moving
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AParagonAssetCharacter::Move);

			// Looking
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AParagonAssetCharacter::Look);

			// Weapon
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AParagonAssetCharacter::WeaponStart);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AParagonAssetCharacter::WeaponStop);

			// Zoom
			EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &AParagonAssetCharacter::AimStart);
			EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &AParagonAssetCharacter::AimStop);

			// 여기서부터 UI 키 바인딩이요!!
			if (MyPlayerController->PauseMenuAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->PauseMenuAction, ETriggerEvent::Started, MyPlayerController, &AMyPlayerController::ToggleGamePauseMenu);
			}

			if (MyPlayerController->InventoryAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->InventoryAction, ETriggerEvent::Started, MyPlayerController, &AMyPlayerController::ToggleInventory);
			}

			if (MyPlayerController->MissionAction)
			{
				EnhancedInputComponent->BindAction(MyPlayerController->MissionAction, ETriggerEvent::Started, MyPlayerController, &AMyPlayerController::ToggleMission);
			}
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}
	}
}

void AParagonAssetCharacter::SetMediumCharge()
{
	ChargeState = EChargeState::Medium;
	GetWorldTimerManager().SetTimer(ChargeTimer,this, &AParagonAssetCharacter::SetFullCharge,ChargeTime, false);
}

void AParagonAssetCharacter::SetFullCharge()
{
	ChargeState = EChargeState::Full;
}

void AParagonAssetCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MovementVector.X))
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.X);
	}

	if (!FMath::IsNearlyZero(MovementVector.Y))
	{
		AddMovementInput(GetActorRightVector(), MovementVector.Y);
	}
}

void AParagonAssetCharacter::Jump()
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	Super::Jump();
}

void AParagonAssetCharacter::StopJumping()
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;
	Super::StopJumping();
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

	ZoomState = EZoomState::Zooming;
	FireState = EFireState::Aiming;
	ZoomStart();
}

void AParagonAssetCharacter::AimStop(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;

	ZoomState = EZoomState::NoZooming;
	UE_LOG(LogTemp, Display, TEXT("FireState = %d"), FireState);
	FireState = EFireState::Waiting;
	
	ZoomStop();
}

void AParagonAssetCharacter::WeaponStart(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;

	FireState = EFireState::Aiming;
	
	GetWorldTimerManager().SetTimer(ChargeTimer, this, &AParagonAssetCharacter::SetMediumCharge,ChargeTime, false);
	UE_LOG(LogTemp, Log, TEXT("WeaponStart"));
}

void AParagonAssetCharacter::WeaponStop(const FInputActionValue& Value)
{
	if (!Controller) return;
	if (HealthState == EHealthState::Dead) return;

	FireState = EFireState::AimingEnd;

	Fire(Value);

	GetWorldTimerManager().ClearTimer(ChargeTimer);
	
	ChargeState = EChargeState::Normal;
	UE_LOG(LogTemp, Log, TEXT("WeaponStop"));
}

// void WeaponStart// 왼클릭
// {
// 	WeaponponComponent->WeaponStart
// 	// WeaponType으로 switch
// }
//
// void PressWeaponEnd()
// {
// 	// Timer -> 일정 시간동안 실행. Fire()
// 		
// 	// 연발하는 애니메이션을 따로 만들수도 있다.
// 	// 
// 		
// 	// WeaponType으로 switch
// }
//
// void AParagonAssetCharacter::Fire()
// {
// 	if (나쏜다)
// 	{
// 		// 몽타주 실행, 몽타주 끝날때 나쏜다 set false
// 		// 몽타주가 실행 중일 때는 또 몽타주가 실행되지 않도록 해야함
// 	}
// }
//
// WeaponComp // RapidFire, 단발, 투척
// {
// public: 
// 	void WeaponStart() // 
// 	{
// 		// RapidFire -> SetTimer(Fire())
// 	}
//
// 	void WeaponEnd()
// 	{
// 		// RapidFire -> ClearTimer(Fire())
// 		// 단발 -> Fire()
// 	}
// 	
// protected:
// 	// 부착물, 무기정보,
// 	// 나쏜다 변수
// 	
// 	void Fire()
// 	{
// 		// Bullet 스폰, 발사
// 		// 나쏜다 set true
// 		
// 		// GetOwner()->Fire()
// 	}
// }


void AParagonAssetCharacter::ZoomStart()
{
	CameraTimelineComponent->Play();
}

void AParagonAssetCharacter::ZoomStop()
{
	CameraTimelineComponent->Reverse();
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
		float MaxYOffset = 0.0f;   // 기본 위치
		float NewYOffset = FMath::Lerp(MaxYOffset, MinYOffset, Alpha);
		CameraBoom->SocketOffset.Y = NewYOffset;
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
