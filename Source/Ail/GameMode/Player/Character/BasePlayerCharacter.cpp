// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/ToolChildActorComponent.h"
#include "Misc/Utils.h"

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


	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SocketOffset = FVector(0.f, 20.f, -10.f);
	SpringArm->ProbeSize = 1.f;
	SpringArm->TargetArmLength = -50.0f;
	SpringArm->ProbeChannel = ECC_WorldStatic;
	SpringArm->bUsePawnControlRotation = true;

	ToolChildActorComponent = CreateDefaultSubobject<UToolChildActorComponent>(TEXT("ToolChildActorComponent"));
	ToolChildActorComponent->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Temp
	FUtils::ChangeSupportUVFromHitResults(false);
}

void ABasePlayerCharacter::OnConstruction(const FTransform& Transform)
{
	//@TODO
	//함수로 변경해서 UI에서 툴의 크기 조절 
	//게임내 모델링 기능 추가 고려하기
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

	UE_LOG(LogTemp, Warning, TEXT("PreToolRotation at: %s"), *InRotation.ToString());
	ToolMesh->SetRelativeRotation(InRotation);
	UE_LOG(LogTemp, Warning, TEXT("CurToolRotation at: %s"), *InRotation.ToString());
}

void ABasePlayerCharacter::SetToolScale3D(FVector InScale)
{
	ToolMesh->SetRelativeScale3D(InScale);
}

