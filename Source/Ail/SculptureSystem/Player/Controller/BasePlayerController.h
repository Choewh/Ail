// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Misc/Utils.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AIL_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	/**
	 * Overridable native function for when this controller is asked to possess a pawn.
	 * @param InPawn The Pawn to be possessed
	 */

protected:

	void OnTest(const FInputActionValue& InputActionValue);
	void OnMove(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);
	void OnUpDown(const FInputActionValue& InputActionValue);
	void OnLeftClick(const FInputActionValue& InputActionValue);
	void OnRightClick(const FInputActionValue& InputActionValue);
	void OnTest(const FInputActionValue& InputActionValue);

	void OnWheelScroll(const FInputActionValue& InputActionValue);
	//void OnZoomWheel(const FInputActionValue& InputActionValue);

protected:
	UInputMappingContext* IMC_Default = nullptr;
	UTexture2D* T_Paint = nullptr;
};
