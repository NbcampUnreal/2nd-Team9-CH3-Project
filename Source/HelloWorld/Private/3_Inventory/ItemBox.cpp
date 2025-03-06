// Copyright Epic Games, Inc. All Rights Reserved.


#include "3_Inventory/ItemBox.h"

#include "0_Framework/MyGameInstance.h"
#include "3_Inventory/InventoryManager.h"
#include "3_Inventory/ItemBase.h"
#include "3_Inventory/WeaponComponent.h"
#include "4_Character/ParagonAssetCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AItemBox::AItemBox()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/ModularSciFi/StaticMeshes/Containers/SM_Box_2.SM_Box_2"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
	}
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnInteract);
}

void AItemBox::OnInteract(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		this->AccquireItem();
		Destroy();
	}
}

void AItemBox::AccquireItem()
{
	if (ItemID.Compare("Empty") == 0) return;
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		// 인벤토리 조사
		if (UInventoryManager* IM = MyGameInstance->GetInventoryManager())
		{
			if (const UItemBase* Item = IM->GetItemFromID(ItemID))
			{
				// Item->AcquireItem();
				ACharacter* PlayerCharacter =  UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
				AParagonAssetCharacter* PlayerParagon = Cast<AParagonAssetCharacter>(PlayerCharacter);
				if (PlayerParagon)
				{
					PlayerParagon->GetCurrentWeapon()->EquipParts(Item);
				}
				UE_LOG(LogTemp, Warning, TEXT("Player Gets '%s'."), *Item->GetItemName().ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ITEM '%s' NOT FOUND"), *ItemID.ToString());
			}
		}
	}
}

