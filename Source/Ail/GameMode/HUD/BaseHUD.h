// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

};
 