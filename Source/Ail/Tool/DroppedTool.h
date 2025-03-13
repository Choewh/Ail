// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Action/Tool/ToolAction.h"
#include "DroppedTool.generated.h"

UCLASS()
class AIL_API ADroppedTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroppedTool();
	ADroppedTool(const FObjectInitializer& ObjectInitializer);
	virtual void UseTool(APlayerController* PC);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// [Root] ShapeComponent (기본은 Sphere로 만들어지며, 자식에서 변경하고자 하면 변경 하시오)
	// 참고: ObjectInitializer.SetDefaultSubobjectClass<UBoxComponent>
	// - SkeletalMeshComponent
	// - StaticMeshComponent
	//	참고: SkeletalMesh 또는 StaticMesh 둘 중 하나를 사용할 수 있도록 2개를 만들어둠
	
	UPROPERTY(VisibleAnywhere)
	UShapeComponent* Collider;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

protected:
	//UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.DroppedItemTableRow"))
	//FDataTableRowHandle DataTableRowHandle;
	
};
