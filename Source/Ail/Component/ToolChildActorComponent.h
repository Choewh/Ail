// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "ToolChildActorComponent.generated.h"

/**
 * 
 */
UCLASS()
class AIL_API UToolChildActorComponent : public UChildActorComponent
{
	GENERATED_BODY()
	
	UToolChildActorComponent();

public:
	virtual void SetData();
};
