// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/DroppedTool.h"

// Sets default values
ADroppedTool::ADroppedTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ADroppedTool::ADroppedTool(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<USphereComponent>(TEXT("Collider")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UShapeComponent>(TEXT("Collider"));
	Collider->SetCollisionProfileName(TEXT("Player"));
	RootComponent = Collider;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADroppedTool::UseTool(APlayerController* PC)
{
	UToolAction* ToolAction = nullptr;
	TUniquePtr<UToolAction::FContext> Context = MakeUnique<UToolAction::FContext>();
	Context->User = PC;
	ToolAction->SetContext(MoveTemp(Context));
	ToolAction->Use();
}

void ADroppedTool::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void ADroppedTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroppedTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

