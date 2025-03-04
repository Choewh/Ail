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

#include "Kismet/KismetMathLibrary.h"

#include "Test/TestActor.h"



ABaseSculpture::ABaseSculpture()
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

void ABaseSculpture::BeginPlay()
{
	Super::BeginPlay();
}


//TODO 실행되면 제자리에 복사해서 스태틱매쉬 생성해주기 바꿔주기
void ABaseSculpture::ConvertMeshDynamicToStatic()
{
	if (!DynamicMeshComponent || !DynamicMeshComponent->GetDynamicMesh())
	{
		return;
	}

	UDynamicMesh* DynamicMesh = DynamicMeshComponent->GetDynamicMesh();

	// 새로운 액터 생성
	FActorSpawnParameters SpawnParams;
	ATestActor* NewPaintingActor = GetWorld()->SpawnActor<ATestActor>(ATestActor::StaticClass(), FTransform::Identity, SpawnParams);
	if (!NewPaintingActor)
	{
		return;
	}

	// 새로운 StaticMesh 생성
	UStaticMesh* NewStaticMesh = NewPaintingActor->MeshComponent->GetStaticMesh();
	if (!NewStaticMesh)
	{
		return;
	}

	// 복사 작업 수행
	FGeometryScriptCopyMeshToAssetOptions Options;
	FGeometryScriptMeshWriteLOD TargetLOD;
	EGeometryScriptOutcomePins Outcome;
	UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshToStaticMesh(DynamicMesh, NewStaticMesh, Options, TargetLOD, Outcome);

	// 위치 조정
	FVector BoundsExtent = NewPaintingActor->MeshComponent->Bounds.BoxExtent;
	NewPaintingActor->MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, BoundsExtent.Z));

	// 가시성 조정
	DynamicMeshComponent->SetVisibility(false);

	// 다이내믹 메시 액터 처리 (개발 단계에서는 삭제, 나중에 재활용 가능하도록 비활성 코드 주석 처리)
	AActor* DynamicMeshOwner = DynamicMeshComponent->GetOwner();
	if (DynamicMeshOwner)
	{
		DynamicMeshOwner->Destroy(); // 개발 단계에서 삭제

		/* 재활용 시 사용할 코드
		DynamicMeshOwner->SetActorEnableCollision(false);
		DynamicMeshOwner->SetActorHiddenInGame(true);
		DynamicMeshOwner->SetActorTickEnabled(false);
		*/
	}
}


void ABaseSculpture::DigSculpture(const UStaticMeshComponent* InMesh, const FTransform& InTransform)
{
	// TODO 최적화 해보기
	UDynamicMesh* ToolDynamicMesh = AllocateComputeMesh();

	if (!DynamicMeshComponent || !DynamicMeshComponent->GetDynamicMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("DynamicMeshComponent or DynamicMesh is nullptr!"));
		return;
	}

	if (!InMesh || !InMesh->GetStaticMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("InMesh or StaticMesh is nullptr!"));
		return;
	}

	{
		FGeometryScriptPrimitiveOptions Options;

		FVector Location = UKismetMathLibrary::InverseTransformLocation(DynamicMeshComponent->GetComponentTransform(), InTransform.GetLocation());

		FTransform NewTransform = InTransform;
		NewTransform.SetLocation(Location);

		FVector FullSize = InMesh->GetStaticMesh()->GetBoundingBox().GetSize();

		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(ToolDynamicMesh, Options, NewTransform, FullSize.X, FullSize.Y, FullSize.Z, 0, 0, 0, EGeometryScriptPrimitiveOriginMode::Center);
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
	ReleaseAllComputeMeshes();
}
