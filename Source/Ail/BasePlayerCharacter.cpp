// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->DefaultLandMovementMode = MOVE_Flying;
	MovementComponent->BrakingDecelerationFlying = 5000.f;

	UCapsuleComponent* PlayerCapsuleComponent = GetCapsuleComponent();
	PlayerCapsuleComponent->SetCapsuleSize(10.f, 10.f);


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->ProbeSize = 0.f;
	SpringArm->TargetArmLength = -50.0f;
	SpringArm->ProbeChannel = ECC_WorldStatic;
	SpringArm->bUsePawnControlRotation = true;
	FVector Temp = SpringArm->GetUnfixedCameraPosition();

	ToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ToolMesh->SetupAttachment(SpringArm);
	ToolMesh->SetCollisionProfileName(TEXT("Guide"));

	// 중앙을 BLocation에 맞추기

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/SculptureSystem/Tool/SculptureBox.SculptureBox'"));

	if (MeshAsset.Succeeded())
	{
		ToolMesh->SetStaticMesh(MeshAsset.Object);
		ToolMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
		ToolMesh->bRenderCustomDepth = true;

	}

}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABasePlayerCharacter::OnConstruction(const FTransform& Transform)
{
	//@TODO
	//함수로 변경해서 UI에서 툴의 크기 조절 
	//게임내 모델링 기능 추가 고려하기
	ToolMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	Super::OnConstruction(Transform);
}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePlayerCharacter::SetToolTransform(FTransform InTransform)
{
	//if(InTransform.GetLocation() == ToolMesh)
	if (!ToolMesh->GetComponentTransform().TranslationEquals(InTransform))
	{
		SetToolLocation(InTransform.GetLocation());
	}

	// 현재 회전값과 다를 때만 적용
	if (!ToolMesh->GetComponentTransform().RotationEquals(InTransform))
	{
		SetToolRotation(InTransform.GetRotation());
	}

	if (!ToolMesh->GetComponentTransform().Scale3DEquals(InTransform))
	{
		SetToolScale3D(InTransform.GetScale3D());
	}
}

void ABasePlayerCharacter::SetToolLocation(FVector InLocation)
{
	ToolMesh->SetRelativeLocation(InLocation);
}

void ABasePlayerCharacter::SetToolRotation(FQuat InRotation)
{
	ToolMesh->SetRelativeRotation(InRotation);
}

void ABasePlayerCharacter::SetToolScale3D(FVector InScale)
{
	ToolMesh->SetRelativeScale3D(InScale);
}

