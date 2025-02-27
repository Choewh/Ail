// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseSculpture.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/CollisionFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"

#include "GeometryScript/MeshRepairFunctions.h"

#include "Selections/MeshConnectedComponents.h"

//#include "MeshQueries.h"
//#include "StaticMeshAttributes.h"


#include "Kismet/KismetMathLibrary.h"

ABaseSculpture::ABaseSculpture(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

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
		TargetDynamicMesh->ProcessMesh([&](const FDynamicMesh3& ReadMesh) {
			UE::Geometry::FMeshConnectedComponents Components(&ReadMesh);
			Components.FindConnectedTriangles(); //연결된 메쉬 찾기

			//FDynamicMesh3 EditableMesh = ReadMesh;
			UE::Geometry::FMeshConnectedComponents::FComponent MinComponent;
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

			});

	}

	ReleaseAllComputeMeshes();

}
