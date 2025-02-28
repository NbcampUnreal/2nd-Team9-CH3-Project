// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DevPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class HELLOWORLD_API ADevPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADevPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ChooseWeapon1Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ChooseWeapon2Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ChooseWeapon3Action;

	virtual void BeginPlay() override;
};
