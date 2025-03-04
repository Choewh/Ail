// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class AIL_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	virtual void DrawBrush(UTexture2D* BurshTexture, float BurshSize, FVector2D DrawLocation, FLinearColor BrushColor);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void RenderTargetInit();
	void							DrawMaterial(UCanvas* Canvas, UMaterialInterface* RenderMaterial, FVector2D ScreenPosition, FVector2D ScreenSize,
		FVector2D CoordinatePosition, FVector2D CoordinateSize = FVector2D::UnitVector, float Rotation = 0.f,
		FVector2D PivotPoint = FVector2D(0.5f, 0.5f)) const;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UStaticMeshComponent* MeshComponent;
	UMaterialInterface* M_Canvas;
	UMaterialInterface* M_Brush;
	UMaterialInstanceDynamic* BrushMaterial;

	UTextureRenderTarget2D* RenderTarget;
};
