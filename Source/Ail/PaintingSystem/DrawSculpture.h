// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/BodySetup.h"
#include "DrawSculpture.generated.h"



UCLASS()
class AIL_API ADrawSculpture : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADrawSculpture();

	//virtual void DrawBrush(UTexture2D* BurshTexture, float BurshSize, FVector2D DrawLocation, FLinearColor BrushColor);

	virtual void FillFromTriMesh(const FTriMeshCollisionData& TriangleMeshDesc, FBodySetupUVInfo& UVInfo);

public:

	UFUNCTION(BlueprintCallable, Category = "Sculpture")
	virtual void DrawBrush(float InBrushSize, FVector2D InDrawLocation, FLinearColor InBrushColor);
	UFUNCTION(BlueprintCallable, Category = "Sculpture")
	virtual void RenderTargetInit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Serialize(FArchive& Ar) override;

	void							DrawMaterial(UCanvas* Canvas, UMaterialInterface* RenderMaterial, FVector2D ScreenPosition, FVector2D ScreenSize,
		FVector2D CoordinatePosition, FVector2D CoordinateSize = FVector2D::UnitVector, float Rotation = 0.f,
		FVector2D PivotPoint = FVector2D(0.5f, 0.5f)) const;
public:
	UStaticMeshComponent* MeshComponent;
	UMaterialInterface* M_Canvas;
	UMaterialInterface* M_Brush;
	UMaterialInstanceDynamic* BrushMaterialInstance;

	UTextureRenderTarget2D* RenderTarget;
};
