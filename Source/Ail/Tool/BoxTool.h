// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool/ToolBase.h"
#include "BoxTool.generated.h"

/**
 * 
 */
UCLASS()
class AIL_API ABoxTool : public AToolBase
{
	GENERATED_BODY()

	ABoxTool();

public:
	virtual void SetData() override;
private:
	virtual void OnUse() override; // 상속받
	
};
