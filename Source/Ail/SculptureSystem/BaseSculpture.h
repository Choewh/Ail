// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "BaseSculpture.generated.h"

/**
 * 
 */

class UTextureRenderTarget2D;

UCLASS()
class AIL_API ABaseSculpture : public AGeneratedDynamicMeshActor
{
	GENERATED_BODY()
	
public:
	ABaseSculpture();

public:
	virtual void BeginPlay();

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void DigSculpture(const UStaticMeshComponent* InMesh, const FTransform& InTransform);
	
	//제자리에 바꿔서 스태틱 매쉬 생성
	virtual void ConvertMeshDynamicToStatic();

};

