// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "ToolWidget.generated.h"

/**
 * 
 */
class ADroppedTool;

UCLASS()
class AIL_API UToolWidget : public UUserWidget
{
	GENERATED_BODY()
	
	friend class ABaseHUD;

protected:
	void AddDroppedTool(ADroppedTool* NewTool);
	void RemoveDroppedTool(ADroppedTool* NewTool);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AddDroppedTool"))
	void ReceiveAddDroppedTool();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "RemoveDroppedTool"))
	void ReceiveRemoveDroppedTool();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* DroppedToolDescVerticalBox = nullptr;
};
