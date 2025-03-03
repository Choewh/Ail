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
	virtual void DrawBrush(UTexture2D* BurshTexture, float BurshSize, FVector2D DrawLocation, FLinearColor BrushColor);
	//void RemoveConnectedMesh(FDynamicMesh3& Mesh, const FVector3d& Point);

	virtual void ConvertMeshDynamicToStatic();
private:
	virtual void RenderTargetInit();

	void							DrawMaterial(UCanvas* Canvas, UMaterialInterface* RenderMaterial, FVector2D ScreenPosition, FVector2D ScreenSize,
		FVector2D CoordinatePosition, FVector2D CoordinateSize = FVector2D::UnitVector, float Rotation = 0.f,
		FVector2D PivotPoint = FVector2D(0.5f, 0.5f)) const;

private:
	
	UStaticMeshComponent* PaintingMeshComponent;
	UMaterialInterface* M_Canvas;
	UMaterialInterface* M_Brush;
	UMaterialInstanceDynamic* BrushMaterial;

	UTextureRenderTarget2D* RenderTarget;

};

