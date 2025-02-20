// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseSculpture.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/CollisionFunctions.h"

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
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(DynamicMesh, Options, T, 200.f, 200.f, 200.f);
	}

	{
		FGeometryScriptPNTessellateOptions Options;
		UGeometryScriptLibrary_MeshSubdivideFunctions::ApplyPNTessellation(DynamicMesh, Options);
	}
	
	bResetOnRebuild = false;

	DynamicMeshComponent->SetComplexAsSimpleCollisionEnabled(true,true);
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

		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(ToolDynamicMesh, Options, T, 20.f, 20.f, 20.f, 0,0,0,EGeometryScriptPrimitiveOriginMode::Center);
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

	ReleaseAllComputeMeshes();
}
