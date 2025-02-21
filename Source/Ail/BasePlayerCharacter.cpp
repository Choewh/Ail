// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->DefaultLandMovementMode = MOVE_Flying;
	MovementComponent->BrakingDecelerationFlying = 5000.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
		
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->ProbeSize = 0.0f;
	SpringArm->TargetArmLength = -300.0f;
	SpringArm->ProbeChannel = ECC_WorldStatic;
 	SpringArm->bUsePawnControlRotation = true;


    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SpringArm);
	StaticMesh->SetCollisionProfileName(TEXT("Guide"));
	
	// 중앙을 BLocation에 맞추기

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"));
    if (MeshAsset.Succeeded())
    {
        StaticMesh->SetStaticMesh(MeshAsset.Object);
    }
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABasePlayerCharacter::OnConstruction(const FTransform& Transform)
{
	FBox BoundingBox = StaticMesh->Bounds.GetBox();
	FVector BoxExtent =	BoundingBox.GetExtent();

	UE_LOG(LogTemp, Warning, TEXT("BoxExtent %s"), *BoxExtent.ToString());

	
	StaticMesh->SetRelativeLocation(-(BoxExtent));

	StaticMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
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

