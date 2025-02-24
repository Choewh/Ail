// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
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
	PlayerCapsuleComponent->SetCapsuleSize(10.f,10.f);


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
		
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->ProbeSize = 0.0f;
	SpringArm->TargetArmLength = -50.0f;
	SpringArm->ProbeChannel = ECC_WorldStatic;
 	SpringArm->bUsePawnControlRotation = true;
	FVector Temp = SpringArm->GetUnfixedCameraPosition();

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SpringArm);
	StaticMesh->SetCollisionProfileName(TEXT("Guide"));
	
	// 중앙을 BLocation에 맞추기

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/SculptureSystem/Tool/SculptureConeTool.SculptureConeTool'"));

    if (MeshAsset.Succeeded())
    {
        StaticMesh->SetStaticMesh(MeshAsset.Object);
		StaticMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
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
	StaticMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
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

