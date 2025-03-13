// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Utils.h"
#include "ToolBase.generated.h"

class AToolBase;

USTRUCT()
struct AIL_API FToolTableRow : public FTableRowBase
{
	GENERATED_BODY()

public: // Base
	UPROPERTY(EditAnywhere, Category = "Tool") // 현재는 패스~
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Tool")
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Tool")
	TSubclassOf<AToolBase> ToolClass;

//public: // Animation
	//UPROPERTY(EditAnywhere, Category = "Tool|Animation")
	//TSubclassOf<UBaisicCharacterAnimInstance> AnimClass; // SkeletalMesh 툴이라면 애니메이션추가

	//UPROPERTY(EditAnywhere, Category = "Tool|Animation")
	//UAnimMontage* ToolAttackMontage;
};

UCLASS()
class AIL_API AToolBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AToolBase();
public:
	UFUNCTION(BlueprintCallable, Category = "Tool")
	USkeletalMeshComponent* GetSkeletalMesh() { return SkeletalMeshComponent; }
	UFUNCTION(BlueprintCallable, Category = "Tool")
	UStaticMeshComponent* GetStaticMesh() { return StaticMeshComponent; }
	UFUNCTION(BlueprintCallable, Category = "Tool")
	virtual void SetToolTransform(FTransform InTransform);

	//데이터에 필요한것? 효과음 메쉬형태 뭐 기타등등이겠지만 일단 매쉬만 바꿀수 있도록
	UFUNCTION(BlueprintCallable, Category = "Tool")
	virtual void SetData();

protected: // Owner
	// 이 도구의 소유권자 Pawn
	APawn* OwningPawn = nullptr;
	// 스프링암이 필요한가 ? 스프링암에 달아줄거라 필요없을지도~
	
	
	//UBaisicCharacterAnimInstance* BasicAnimInstance = nullptr;
	friend class UToolAction_Tool;
//UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.WeaponTableRow"))
//FDataTableRowHandle DataTableRowHandle;

const FToolTableRow* ToolTableRow = nullptr;

protected: // Input
	class UInputMappingContext* IMC_Tool = nullptr;

	//PROPERTY
private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;
	//FUNCTION
private:
	virtual void OnUse(const FInputActionValue& InputActionValue); // 상속받아 사용해서 툴마다 다른 사용ㄱ

	virtual void SetToolLocation(FVector InLocation);
	virtual void SetToolRotation(FQuat InRotation);
	virtual void SetToolScale3D(FVector InScale);
};
