// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingSystem/BaseDrawSculpture.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"

// Sets default values
ABaseDrawSculpture::ABaseDrawSculpture()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
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
void ABaseDrawSculpture::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseDrawSculpture::RenderTargetInit()
{
	{
		RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024);
		UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RenderTarget, FLinearColor(1, 1, 1, 1));
	}

	{
		UMaterialInstanceDynamic* MaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), M_Canvas);
		MaterialDynamicInstance->SetTextureParameterValue(TEXT("RenderTarget"), RenderTarget); //베이스 컬러
		MeshComponent->SetMaterial(0, MaterialDynamicInstance);
	}

	{
		BrushMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), M_Brush , FName("BrushMaterialInstance"));
	}
}

//void ABaseDrawSculpture::DrawBrush(UTexture2D* BrushTexture, float BrushSize, FVector2D DrawLocation, FLinearColor BrushColor)
void ABaseDrawSculpture::DrawBrush(float InBrushSize, FVector2D InDrawLocation, FLinearColor InBrushColor)
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

void ABaseDrawSculpture::DrawMaterial(UCanvas* Canvas, UMaterialInterface* RenderMaterial, FVector2D ScreenPosition, FVector2D ScreenSize, FVector2D CoordinatePosition, FVector2D CoordinateSize, float Rotation, FVector2D PivotPoint) const
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

// Called every frame
void ABaseDrawSculpture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

