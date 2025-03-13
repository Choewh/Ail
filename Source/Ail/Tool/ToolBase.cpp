#include "ToolBase.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AToolBase::AToolBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("Guide"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(TEXT("Guide"));
	// Input
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Tool/Input/IMC_Tool.IMC_Tool'") };
		check(Asset.Object);

		IMC_Tool = Asset.Object;
	}

	// 중앙을 BLocation에 맞추기

	//Data에서 설정해주기ㅇㅇ
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/SculptureSystem/Tool/SculptureBox.SculptureBox'"));

	//if (MeshAsset.Succeeded())
	//{
	//	ToolMesh->SetStaticMesh(MeshAsset.Object);
	//	ToolMesh->bRenderCustomDepth = true;
	//}
}

void AToolBase::SetToolTransform(FTransform InTransform)
{
	//if(InTransform.GetLocation() == ToolMesh)
	if (!StaticMeshComponent->GetComponentTransform().TranslationEquals(InTransform))
	{
		SetToolLocation(InTransform.GetLocation());
	}
	// 현재 회전값과 다를 때만 적용
	if (!StaticMeshComponent->GetComponentTransform().RotationEquals(InTransform))
	{
		SetToolRotation(InTransform.GetRotation());
	}

	if (!StaticMeshComponent->GetComponentTransform().Scale3DEquals(InTransform))
	{
		SetToolScale3D(InTransform.GetScale3D());
	}
}

void AToolBase::SetData()
{
	/*DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FWeaponTableRow* Data = DataTableRowHandle.GetRow<FWeaponTableRow>(TEXT("Weapon"));
	if (!Data) { ensure(false); return; }
	WeaponTableRow = Data;

	SkeletalMeshComponent->SetSkeletalMesh(Data->SkeletalMesh);
	SkeletalMeshComponent->SetRelativeTransform(Data->Transform);
	check(Data->AnimClass);
	USkeletalMeshComponent* MeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	check(MeshComponent);
	MeshComponent->SetAnimClass(Data->AnimClass);

	{
		OwningPawn = Cast<APawn>(GetOwner());
		check(OwningPawn);
		OwningPawnBodyMesh = OwningPawn->GetComponentByClass<USkeletalMeshComponent>();
		check(OwningPawnBodyMesh);
		BasicAnimInstance = Cast<UBaisicCharacterAnimInstance>(MeshComponent->GetAnimInstance());
		check(BasicAnimInstance);

		OwnerStatusComponent = OwningPawn->GetComponentByClass<UStatusComponent>();
		check(OwnerStatusComponent);
		{
			BasicAnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnMontageEnd);
		}
	}*/

	if (APlayerController* PlayerController = Cast<APlayerController>(OwningPawn->GetController()))
	{
		check(PlayerController);

		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			Subsystem->AddMappingContext(IMC_Tool, 0);
			EnableInput(PlayerController);
			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
			ensure(EnhancedInputComponent);
			if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Tool, TEXT("IA_UseTool")))
			{
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnUse);
			}
		}
	}
}

void AToolBase::OnUse(const FInputActionValue& InputActionValue)
{
}

void AToolBase::SetToolLocation(FVector InLocation)
{
	StaticMeshComponent->SetRelativeLocation(InLocation);
}

void AToolBase::SetToolRotation(FQuat InRotation)
{
	StaticMeshComponent->SetRelativeRotation(InRotation);
}

void AToolBase::SetToolScale3D(FVector InScale)
{
	StaticMeshComponent->SetRelativeScale3D(InScale);
}


