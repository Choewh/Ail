// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawSculpture.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/TextureRenderTarget2D.h"

#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsCore.h"
//Interface_CollisionDataProviderCore.h	"

#include "Engine/Canvas.h"

// Sets default values
ADrawSculpture::ADrawSculpture()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/PaintingSystem/Cube.Cube'"));

	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}

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

	MeshComponent->SetCollisionProfileName(TEXT("Sculpture"));
}

// Called when the game starts or when spawned
void ADrawSculpture::BeginPlay()
{
	Super::BeginPlay();
	RenderTargetInit();
}

void ADrawSculpture::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

}

void ADrawSculpture::RenderTargetInit()
{
	{
		RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024);
		UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RenderTarget, FLinearColor(1, 1, 1, 1));
	}

	{
		UMaterialInstanceDynamic* MaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), M_Canvas);
		MaterialDynamicInstance->SetTextureParameterValue(TEXT("RenderTarget"), RenderTarget); //베이스 컬러
		MeshComponent->SetMaterial(0, MaterialDynamicInstance);

		////Temp
		//UBodySetup* BodySetup = MeshComponent->GetBodySetup();
		//BodySetup->ClearPhysicsMeshes();
		//UStaticMesh* OwnerStaticMesh = MeshComponent->GetStaticMesh();
		//FTriMeshCollisionData CollisionData;
		//if (OwnerStaticMesh->ContainsPhysicsTriMeshData(true))
		//{
		//	OwnerStaticMesh->GetPhysicsTriMeshData(&CollisionData, true);
		//}
		//FillFromTriMesh(CollisionData, BodySetup->UVInfo);
		//int32 UVChannel = 0;
		//FBodySetupUVInfo FUVInfo = BodySetup->UVInfo;
		//// UV 채널 유효성 검사
		//if (!FUVInfo.VertUVs.IsValidIndex(UVChannel))
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Invalid UV Channel: %d (Max: %d)"), UVChannel, FUVInfo.VertUVs.Num());
		//}
		////}

	}
	{
		BrushMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), M_Brush, FName("BrushMaterialInstance"));
	}
}

void ADrawSculpture::FillFromTriMesh(const FTriMeshCollisionData& TriangleMeshDesc, FBodySetupUVInfo& UVInfo)
{
	// Store index buffer
	const int32 NumVerts = TriangleMeshDesc.Vertices.Num();
	const int32 NumTris = TriangleMeshDesc.Indices.Num();
	UVInfo.IndexBuffer.Empty();
	UVInfo.IndexBuffer.AddUninitialized(NumTris * 3);
	for (int32 TriIdx = 0; TriIdx < TriangleMeshDesc.Indices.Num(); TriIdx++)
	{
		UVInfo.IndexBuffer[TriIdx * 3 + 0] = TriangleMeshDesc.Indices[TriIdx].v0;
		UVInfo.IndexBuffer[TriIdx * 3 + 1] = TriangleMeshDesc.Indices[TriIdx].v1;
		UVInfo.IndexBuffer[TriIdx * 3 + 2] = TriangleMeshDesc.Indices[TriIdx].v2;
	}

	// Store vertex positions
	UVInfo.VertPositions.Empty();
	UVInfo.VertPositions.AddUninitialized(NumVerts);
	for (int32 VertIdx = 0; VertIdx < TriangleMeshDesc.Vertices.Num(); VertIdx++)
	{
		UVInfo.VertPositions[VertIdx] = FVector3d(TriangleMeshDesc.Vertices[VertIdx]);
	}

	// Copy UV channels (checking they are correct size)
	for (int32 UVIndex = 0; UVIndex < TriangleMeshDesc.UVs.Num(); UVIndex++)
	{
		if (TriangleMeshDesc.UVs[UVIndex].Num() == NumVerts)
		{
			UVInfo.VertUVs.Add(TriangleMeshDesc.UVs[UVIndex]);
		}
		else
		{
			break;
		}
	}
}

//void ADrawSculpture::DrawBrush(UTexture2D* BrushTexture, float BrushSize, FVector2D DrawLocation, FLinearColor BrushColor)
void ADrawSculpture::DrawBrush(float InBrushSize, FVector2D InDrawLocation, FLinearColor InBrushColor)
{
	if (!RenderTarget || !BrushMaterialInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid RenderTarget, BrushMaterialInstance!"));
		return;
	}

	// 브러시 텍스처 및 색상 설정
	//BrushMaterialInstance->SetTextureParameterValue(TEXT("BrushTexture"),BrushTexture);
	//FLinearColor CurrentColor;
	//if (BrushMaterialInstance->GetVectorParameterValue(TEXT("BrushColor"), CurrentColor))
	//{
	BrushMaterialInstance->SetVectorParameterValue(TEXT("BrushColor"), InBrushColor);
	/*}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Parameter 'BrushColor' not found in material!"));
	}*/

	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;

	// 렌더 타겟을 캔버스로 설정
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RenderTarget, Canvas, Size, Context);

	if (Canvas)
	{
		// 캔버스 크기 기반으로 위치 계산
		FVector2D ScreenSize = FVector2D(InBrushSize, InBrushSize);
		FVector2D ScreenPosition = (Size * InDrawLocation) - (InBrushSize * 0.5f); // UV 좌표를 픽셀 좌표로 변환 중앙 정렬

		// 브러시 그리기
		Canvas->K2_DrawMaterial(BrushMaterialInstance, ScreenPosition, ScreenSize, FVector2D::ZeroVector);

		UE_LOG(LogTemp, Warning, TEXT("Drawing Brush at Location: %s"), *ScreenPosition.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Canvas is NULL!"));
	}

	// 렌더 타겟에 그리기 완료
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void ADrawSculpture::DrawMaterial(UCanvas* Canvas, UMaterialInterface* RenderMaterial, FVector2D ScreenPosition, FVector2D ScreenSize, FVector2D CoordinatePosition, FVector2D CoordinateSize, float Rotation, FVector2D PivotPoint) const
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
