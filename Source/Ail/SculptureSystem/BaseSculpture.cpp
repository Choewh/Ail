// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseSculpture.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/CollisionFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "GeometryScript/MeshAssetFunctions.h"

#include "GeometryScript/MeshRepairFunctions.h"

#include "Selections/MeshConnectedComponents.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
//#include "MeshQueries.h"
//#include "StaticMeshAttributes.h"


#include "Kismet/KismetMathLibrary.h"

ABaseSculpture::ABaseSculpture()
{

	PaintingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	PaintingMeshComponent->SetActive(false);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Canvas(TEXT("/Script/Engine.Material'/Game/PaintingSystem/M_Canvas.M_Canvas'"));

	if (Canvas.Succeeded())
	{
		M_Canvas = Canvas.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Brush(TEXT("/Script/Engine.Material'/Game/PaintingSystem/M_Brush.M_Brush'"));

	if (Brush.Succeeded())
	{
		M_Brush = Brush.Object;
	}
}

void ABaseSculpture::OnConstruction(const FTransform& Transform)
{
	UDynamicMesh* DynamicMesh = DynamicMeshComponent->GetDynamicMesh();

	DynamicMesh->Reset();

	{
		FTransform T;
		FGeometryScriptPrimitiveOptions Options;
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(DynamicMesh, Options, T, 100.f, 100.f, 100.f);
	}

	{
		FGeometryScriptPNTessellateOptions Options;
		UGeometryScriptLibrary_MeshSubdivideFunctions::ApplyPNTessellation(DynamicMesh, Options);
	}

	bResetOnRebuild = false;

	DynamicMeshComponent->SetComplexAsSimpleCollisionEnabled(true, true);
	DynamicMeshComponent->SetCollisionProfileName(TEXT("Sculpture"));

	Super::OnConstruction(Transform);
}

void ABaseSculpture::BeginPlay()
{
	Super::BeginPlay();

	RenderTargetInit();

}



void ABaseSculpture::ConvertMeshDynamicToStatic()
{
	UDynamicMesh* DynamicMesh = DynamicMeshComponent->GetDynamicMesh();
	UStaticMesh* PaintingMesh = PaintingMeshComponent->GetStaticMesh();

	FGeometryScriptCopyMeshFromAssetOptions AssetOptions;
	FGeometryScriptMeshReadLOD RequestedLOD;
	EGeometryScriptOutcomePins Outcome;

	UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshFromStaticMeshV2(PaintingMesh, DynamicMesh, AssetOptions, RequestedLOD, Outcome);
	
	DynamicMeshComponent->SetActive(false);
	//UStaticMesh* FromStaticMeshAsset,
	//UDynamicMesh* ToDynamicMesh,
	//FGeometryScriptCopyMeshFromAssetOptions AssetOptions,
	//FGeometryScriptMeshReadLOD RequestedLOD,
	//EGeometryScriptOutcomePins& Outcome,
	//bool bUseSectionMaterials,
	//UGeometryScriptDebug* Debug)
}

void ABaseSculpture::RenderTargetInit()
{
	{
		RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024);
		UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RenderTarget, FLinearColor(1, 1, 1, 1));
	}

	{
		UMaterialInstanceDynamic* MaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), M_Canvas);
		MaterialDynamicInstance->SetTextureParameterValue(TEXT("RenderTarget"), RenderTarget);
		DynamicMeshComponent->SetMaterial(0, MaterialDynamicInstance);
	}

	{
		BrushMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), M_Brush);
	}
}



void ABaseSculpture::DigSculpture(const UStaticMeshComponent* InMesh, const FTransform& InTransform)
{
	UDynamicMesh* ToolDynamicMesh = AllocateComputeMesh();

	//툴모양으로 파내는 기능 //스위치문 넣어서 ㄱ
	{
		FGeometryScriptPrimitiveOptions Options;

		FVector Location = UKismetMathLibrary::InverseTransformLocation(DynamicMeshComponent->GetComponentTransform(), InTransform.GetLocation());

		FTransform NewTransform = InTransform;
		NewTransform.SetLocation(Location);

		FVector FullSize = InMesh->GetStaticMesh()->GetBoundingBox().GetSize();

		//박스
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(ToolDynamicMesh, Options, NewTransform, FullSize.X, FullSize.Y, FullSize.Z, 0, 0, 0, EGeometryScriptPrimitiveOriginMode::Center);
		UE_LOG(LogTemp, Warning, TEXT("InTransform: %s"), *InTransform.ToString());
		UE_LOG(LogTemp, Warning, TEXT("NewTransform: %s"), *NewTransform.ToString());
		UE_LOG(LogTemp, Warning, TEXT("NewTransform: %s"), *NewTransform.Rotator().ToString());

		//콘

		//FRotator ConeRot = FRotator(-90.f, 0.f, 0.f) + InRotation;
		//FTransform ConeT(ConeRot, Location, FVector(1.0, 1.0, 1.0));								//R 1 : H 6 / Scale 0.1f
		//UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCone(ToolDynamicMesh, Options, ConeT, 5.f, 0.f, 30.f , 12 , 0 , true, EGeometryScriptPrimitiveOriginMode::Base);

		//원
		//UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendSphereBox(ToolDynamicMesh, Options, T, 25.f , 0 , 0 , 0 , EGeometryScriptPrimitiveOriginMode::Center);
	}

	UDynamicMesh* TargetDynamicMesh = DynamicMeshComponent->GetDynamicMesh();

	{
		FTransform T;
		FGeometryScriptMeshBooleanOptions Options;
		UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean(TargetDynamicMesh, T, ToolDynamicMesh, T, EGeometryScriptBooleanOperation::Subtract, Options);
	}

	{
		FGeometryScriptCollisionFromMeshOptions Options;
		UGeometryScriptLibrary_CollisionFunctions::SetDynamicMeshCollisionFromMesh(TargetDynamicMesh, DynamicMeshComponent, Options);
	}

	{
		FGeometryScriptRemoveSmallComponentOptions RemoveOptions;
		RemoveOptions.MinVolume = 0.01f; // 최소 부피 설정
		RemoveOptions.MinArea = 1.0f; // 최소 면적 설정
		RemoveOptions.MinTriangleCount = 180; // 최소 삼각형 개수 설정
		UGeometryScriptLibrary_MeshRepairFunctions::RemoveSmallComponents(TargetDynamicMesh, RemoveOptions);
	}

	{
		int32 Num = UGeometryScriptLibrary_MeshQueryFunctions::GetNumConnectedComponents(TargetDynamicMesh);
		UE_LOG(LogTemp, Warning, TEXT("Connected Group : %i"), Num);
	}

	//TODO
	//해당 좌표에서 가장 가까운 삼각형 ID 찾기 
	{
		//TargetDynamicMesh->ProcessMesh([&](const FDynamicMesh3& ReadMesh) {
		//	UE::Geometry::FMeshConnectedComponents Components(&ReadMesh);
		//	Components.FindConnectedTriangles(); //연결된 메쉬 찾기

			//FDynamicMesh3 EditableMesh = ReadMesh;
			//UE::Geometry::FMeshConnectedComponents::FComponent MinComponent;
			//MinComponent = Components[0];

			//{
			//	//Components 배열안의 떨어진 삼각형 메쉬의 수
			//	for (const UE::Geometry::FMeshConnectedComponents::FComponent& Component : Components.Components)
			//	{
			//		//찾은 ID가 속한 배열 메쉬 제거
			//		if (Component.Indices.Num() < INT16_MAX)
			//		{
			//			//배열안의 삼각형들 삭제
			//			for (int32 TriangleID : Component.Indices)
			//			{
			//				EditableMesh.RemoveTriangle(TriangleID, true); // true = 빈 구멍을 남기지 않고 삭제
			//			}
			//		}
			//	}
			//}

			//if (Components.Num() > 1)
			//{
			//	for (const UE::Geometry::FMeshConnectedComponents::FComponent& Component : Components.Components)
			//	{
			//		if (Component.Indices.Num() <= MinComponent.Indices.Num())
			//		{
			//			MinComponent = Component;
			//		}
			//	}
			//	for (int32 TriangleID : MinComponent.Indices)
			//	{
			//		EditableMesh.RemoveTriangle(TriangleID, true); // true = 빈 구멍을 남기지 않고 삭제
			//	}
			//}
			//});

	}

	ReleaseAllComputeMeshes();

}



void ABaseSculpture::DrawBrush(UTexture2D* BurshTexture, float BrushSize, FVector2D DrawLocation, FLinearColor BrushColor)
{
	{
		BrushMaterial->SetTextureParameterValue(TEXT("BrushTexture"), Cast<UTexture>(BurshTexture));
		FLinearColor BrushColor = FLinearColor::MakeRandomColor(); // 예제: 랜덤 색상
		BrushMaterial->SetVectorParameterValue(TEXT("BrushColor"), BrushColor);
	}

	{
		UCanvas* Canvas;
		FVector2D Size;
		FDrawToRenderTargetContext Context;
		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RenderTarget, Canvas, Size, Context);

		Size *= DrawLocation;
		BrushSize /= 2.f;

		FVector2D ScreenPositon = Size - BrushSize;

		FVector2D ScreenSize = FVector2D(BrushSize, BrushSize);

		DrawMaterial(Canvas, BrushMaterial, ScreenPositon, ScreenSize, FVector2D::ZeroVector);

		UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
	}
}

void ABaseSculpture::DrawMaterial(UCanvas* Canvas, UMaterialInterface* RenderMaterial, FVector2D ScreenPosition, FVector2D ScreenSize, FVector2D CoordinatePosition, FVector2D CoordinateSize, float Rotation, FVector2D PivotPoint) const
{
	if (RenderMaterial
		&& ScreenSize.X > 0.0f
		&& ScreenSize.Y > 0.0f
		// Canvas can be NULL if the user tried to draw after EndDrawCanvasToRenderTarget
		&& Canvas)
	{
		FCanvasTileItem TileItem(ScreenPosition, RenderMaterial->GetRenderProxy(), ScreenSize, CoordinatePosition, CoordinatePosition + CoordinateSize);
		TileItem.Rotation = FRotator(0, Rotation, 0);
		TileItem.PivotPoint = PivotPoint;
		TileItem.SetColor(Canvas->DrawColor);
		Canvas->DrawItem(TileItem);
	}
}
