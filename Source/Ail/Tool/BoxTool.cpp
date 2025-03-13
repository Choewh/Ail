// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/BoxTool.h"

ABoxTool::ABoxTool()
{
	ToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ToolMesh->SetCollisionProfileName(TEXT("Guide"));

	// 중앙을 BLocation에 맞추기

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/SculptureSystem/Tool/SculptureBox.SculptureBox'"));

	if (MeshAsset.Succeeded())
	{
		ToolMesh->SetStaticMesh(MeshAsset.Object);
		ToolMesh->bRenderCustomDepth = true;
	}
}

void ABoxTool::SetData()
{
	Super::SetData();
}

void ABoxTool::OnUse()
{
}
