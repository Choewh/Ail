// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseSculpture.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/CollisionFunctions.h"
#include "GeometryScript/MeshAssetFunctions.h"
#include "GeometryScript/MeshDeformFunctions.h"

#include "GeometryScript/MeshRepairFunctions.h"

#include "StaticMeshAttributes.h"

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
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(DynamicMesh, Options, T, 100, 100.f, 100.f);
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

void ABaseSculpture::DigSculpture(const FVector& InLocation, const FRotator& InRotation)
{
	UDynamicMesh* ToolDynamicMesh = AllocateComputeMesh();

	{
		FGeometryScriptPrimitiveOptions Options;

		FVector Location = UKismetMathLibrary::InverseTransformLocation(DynamicMeshComponent->GetComponentTransform(), InLocation);

		//���� �ڽ� ũ�� �޾ƿ��� Box/2 �� ����
		//Location -= FVector(10.f,10.f,10.f);
		FTransform T(InRotation, Location, FVector(1.0, 1.0, 1.0));

		//박스 
		//UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(ToolDynamicMesh, Options, T, 50.f, 50.f, 50.f, 0 , 0 , 0 , EGeometryScriptPrimitiveOriginMode::Center);
		
		//콘
		FRotator ConeRot = FRotator(-90.f, 0.f, 0.f) + InRotation;
		FTransform ConeT(ConeRot, Location, FVector(1.0, 1.0, 1.0));								//R 1 : H 6 / Scale 0.1f
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCone(ToolDynamicMesh, Options, ConeT, 5.f, 0.f, 30.f , 12 , 0 , true, EGeometryScriptPrimitiveOriginMode::Base);

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
		RemoveOptions.MinTriangleCount = 50; // 최소 삼각형 개수 설정
		UGeometryScriptLibrary_MeshRepairFunctions::RemoveSmallComponents(TargetDynamicMesh, RemoveOptions);
	}

	ReleaseAllComputeMeshes();

}



void ABaseSculpture::SplitStaticMeshActorByCollision(AGeneratedDynamicMeshActor* SourceActor, FTriMeshCollisionData* CollisionData, UWorld* World)
{
	if (!SourceActor || !CollisionData || CollisionData->Vertices.Num() == 0 || !World)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Input Data"));
		return;
	}

	// 1️⃣ 두 개의 콜리전 그룹을 나누기
	TArray<FVector3f> Vertices1, Vertices2;
	TArray<FTriIndices> Triangles1, Triangles2;

	for (int32 i = 0; i < CollisionData->Indices.Num(); i++)
	{
		if (i % 2 == 0) // 짝수 인덱스 → 첫 번째 그룹
		{
			Triangles1.Add(CollisionData->Indices[i]);
			Vertices1.Add(CollisionData->Vertices[CollisionData->Indices[i].v0]);
			Vertices1.Add(CollisionData->Vertices[CollisionData->Indices[i].v1]);
			Vertices1.Add(CollisionData->Vertices[CollisionData->Indices[i].v2]);
		}
		else // 홀수 인덱스 → 두 번째 그룹
		{
			Triangles2.Add(CollisionData->Indices[i]);
			Vertices2.Add(CollisionData->Vertices[CollisionData->Indices[i].v0]);
			Vertices2.Add(CollisionData->Vertices[CollisionData->Indices[i].v1]);
			Vertices2.Add(CollisionData->Vertices[CollisionData->Indices[i].v2]);
		}
	}

	// 2️⃣ 각 그룹을 기반으로 새로운 UStaticMesh 생성
	UStaticMesh* StaticMesh1 = CreateStaticMeshFromData(Vertices1, Triangles1);
	UStaticMesh* StaticMesh2 = CreateStaticMeshFromData(Vertices2, Triangles2);

	if (!StaticMesh1 || !StaticMesh2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create Static Meshes"));
		return;
	}

	// 3️⃣ 새로운 StaticMeshActor 생성
	
	FGeometryScriptCopyMeshFromAssetOptions AssetOptions;
	FGeometryScriptMeshReadLOD RequestedLOD;
	EGeometryScriptOutcomePins Outcome;
	
	ABaseSculpture* NewActor1 = World->SpawnActor<ABaseSculpture>();
	
	if (NewActor1)
	{

		UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshFromStaticMeshV2(StaticMesh1, NewActor1->DynamicMeshComponent->GetDynamicMesh(), AssetOptions, RequestedLOD, Outcome);
		NewActor1->SetActorTransform(SourceActor->GetActorTransform());
	}

	ABaseSculpture* NewActor2 = World->SpawnActor<ABaseSculpture>();
	if (NewActor2)
	{
		UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshFromStaticMeshV2(StaticMesh1, NewActor1->DynamicMeshComponent->GetDynamicMesh(), AssetOptions, RequestedLOD, Outcome);
		NewActor2->SetActorTransform(SourceActor->GetActorTransform());
	}

	// 4️⃣ 기존 Actor 제거 (또는 숨김 처리)
	SourceActor->Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Successfully Split StaticMeshActor into Two"));
}

UStaticMesh* ABaseSculpture::CreateStaticMeshFromData(const TArray<FVector3f>& Vertices, const TArray<FTriIndices>& Triangles)
{
	if (Vertices.Num() == 0 || Triangles.Num() == 0)
	{
		return nullptr;
	}

	UStaticMesh* NewStaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), UStaticMesh::StaticClass());
	FMeshDescription MeshDescription;
	FStaticMeshAttributes Attributes(MeshDescription);
	Attributes.Register();

	TArray<FVertexID> VertexIDs;

	// 1️⃣ 버텍스 추가
	for (const FVector3f& Vertex : Vertices)
	{
		FVertexID VertexID = MeshDescription.CreateVertex();
		Attributes.GetVertexPositions()[VertexID] = (FVector3f)Vertex;
		VertexIDs.Add(VertexID);
	}

	// 2️⃣ 삼각형 추가
	for (const FTriIndices& Triangle : Triangles)
	{
		FPolygonGroupID PolygonGroupID = MeshDescription.CreatePolygonGroup();
		TArray<FVertexInstanceID> VertexInstances;
		for (int32 i = 0; i < 3; i++)
		{
			FVertexInstanceID VertexInstanceID = MeshDescription.CreateVertexInstance(VertexIDs[i]);
			VertexInstances.Add(VertexInstanceID);
		}
		MeshDescription.CreatePolygon(PolygonGroupID, VertexInstances);
	}

	// 3️⃣ StaticMesh 생성
	FStaticMeshLODResources LODResources;

	NewStaticMesh->BuildFromMeshDescription(MeshDescription, LODResources);
	return NewStaticMesh;
}


