// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "BaseSculpture.generated.h"

/**
 * 
 */
UCLASS()
class AIL_API ABaseSculpture : public AGeneratedDynamicMeshActor
{
	GENERATED_BODY()
	
public:
	ABaseSculpture(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void DigSculpture(const FVector& InLocation, const FRotator& InRotation);
};
