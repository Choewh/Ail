// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Tool/DroppedTool.h"
#include "Tool/ToolWidget.h"
#include "Misc/Utils.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class AIL_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABaseHUD();
protected:
	void AddDroppedTool(ADroppedTool* NewTool);
	void RemoveDroppedTool(ADroppedTool* NewTool);

protected: // InputAction callback
	void OnPickupTool(const FInputActionValue& InputActionValue);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	class ABasePlayerController* PlayerController = nullptr;
	class UToolWidget* Widget = nullptr;
	UInputMappingContext* IMC_Tool = nullptr;

	TArray<AActor*> OverlappedDroppedTools;
};
 