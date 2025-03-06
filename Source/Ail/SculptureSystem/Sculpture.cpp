// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sculpture.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/CollisionFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "GeometryScript/MeshAssetFunctions.h"
#include "GeometryScript/MeshRepairFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"

#include "Kismet/KismetMathLibrary.h"
#include "PaintingSystem/DrawSculpture.h"
#include "RenderResource.h"
#include "RenderingThread.h"

#include "Misc/Utils.h"

using namespace UE::Geometry;


ASculpture::ASculpture()
{
}

void ASculpture::OnConstruction(const FTransform& Transform)
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

void ASculpture::BeginPlay()
{
	Super::BeginPlay();
}

//TODO 실행되면 제자리에 복사해서 스태틱매쉬 생성해주기 바꿔주기
void ASculpture::ConvertMeshDynamicToStatic()
{
	if (!DynamicMeshComponent || !DynamicMeshComponent->GetDynamicMesh())
	{
		return;
	}

	UDynamicMesh* DynamicMesh = DynamicMeshComponent->GetDynamicMesh();

	if (!DynamicMesh)
	{
		return;
	}

	// 새로운 액터 생성
	FActorSpawnParameters SpawnParams;
	FTransform NewTransform = GetActorTransform();
	ADrawSculpture* NewPaintingActor = GetWorld()->SpawnActor<ADrawSculpture>(ADrawSculpture::StaticClass(), NewTransform, SpawnParams);
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
	{
		//Temp
		UBodySetup* BodySetup = DynamicMeshComponent->GetBodySetup();
		{
			int32 UVChannel = 0;
			FBodySetupUVInfo FUVInfo = BodySetup->UVInfo;
			// UV 채널 유효성 검사
			if (!FUVInfo.VertUVs.IsValidIndex(UVChannel))
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid UV Channel: %d (Max: %d)"), UVChannel, FUVInfo.VertUVs.Num());
			}
		}
	}
	// 다이내믹 매쉬 정렬 후 언래핑
	{
		DynamicMesh->GetMeshPtr()->CompactInPlace();
		FGeometryScriptXAtlasOptions XAtlasOptions;
		DynamicMesh = UGeometryScriptLibrary_MeshUVFunctions::AutoGenerateXAtlasMeshUVs(DynamicMesh, 0, XAtlasOptions);


		//Temp
		UBodySetup* BodySetup = DynamicMeshComponent->GetBodySetup();
		{
			int32 UVChannel = 0;
			FBodySetupUVInfo FUVInfo = BodySetup->UVInfo;
			// UV 채널 유효성 검사
			if (!FUVInfo.VertUVs.IsValidIndex(UVChannel))
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid UV Channel: %d (Max: %d)"), UVChannel, FUVInfo.VertUVs.Num());
			}
		}
	}

	{
		// 복사 작업 수행
		FGeometryScriptCopyMeshToAssetOptions CopyMeshToAssetOptions;
		CopyMeshToAssetOptions.bUseOriginalVertexOrder = true;
		FGeometryScriptMeshWriteLOD TargetLOD;
		EGeometryScriptOutcomePins Outcome;
		UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshToStaticMesh(DynamicMesh, NewStaticMesh, CopyMeshToAssetOptions, TargetLOD, Outcome);
	}

	NewPaintingActor->RenderTargetInit();
	// 가시성 조정
	DynamicMeshComponent->SetVisibility(false);

	//여기서 UV 힛 설정해주기
	//설정후 성공적으로 바꼈는지 확인후 넘어가기.
	if (!FUtils::ChangeSupportUVFromHitResults(true))
	{
		return;
	}

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


void ASculpture::DigSculpture(const UStaticMeshComponent* InMesh, const FTransform& InTransform)
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
		//페인팅 때문에 피직스 옵션에서 힛 UV 결과 추가 를 체크하니 여기서 오류가남
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