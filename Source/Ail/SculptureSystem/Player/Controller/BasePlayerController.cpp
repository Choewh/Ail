// Fill out your copyright notice in the Description page of Project Settings.

#include "SculptureSystem/Player/Controller/BasePlayerController.h"
#include "BasePlayerCharacter.h"
#include "SculptureSystem/BaseSculpture.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DrawDebugHelpers.h"

ABasePlayerController::ABasePlayerController()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/SculptureSystem/Input/IMC_BaseInput.IMC_BaseInput'") };
		check(Asset.Object);

		IMC_Default = Asset.Object;
	}

	//PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(IMC_Default, 0);
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Move")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnMove);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Move is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Look")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnLook);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Look is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_UpDown")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnUpDown);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_UpDown is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_LeftClick")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ABasePlayerController::OnLeftClick);
		//EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnLeftClick);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_LeftClick is disabled"));
	}


	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_MouseWheel")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnWheelScroll);
		//EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnLeftClick);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_MouseWheel is disabled"));
	}
}

void ABasePlayerController::OnMove(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = K2_GetActorRotation();
	const FRotator RotationYaw = FRotator(Rotation.Pitch, Rotation.Yaw, 0.0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
	ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);
}

void ABasePlayerController::OnLook(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	AddYawInput(ActionValue.X);
	AddPitchInput(-ActionValue.Y);
}

void ABasePlayerController::OnUpDown(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	const FVector UpVector = FVector(0.f,0.f,1.f);

	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(UpVector, ActionValue);
}

void ABasePlayerController::OnLeftClick(const FInputActionValue& InputActionValue)
{
	ABasePlayerCharacter* ControlledCharacter = Cast<ABasePlayerCharacter>(GetCharacter());
	//박스용
	//FVector HalfSize = FVector(10.f, 10.f, 10.f);
	//FRotator BoxRotator = FRotator::ZeroRotator;
	//TArray<AActor*> IgnoreActors;
	
	// 툴의 정보를 토대로 콜리전을 생성함
	// 박스 피벗 포지션
	FVector BoxPosition = ControlledCharacter->ToolMesh->GetComponentLocation();

	// 박스 전방 방향 벡터
	FVector ForwardDir = ControlledCharacter->ToolMesh->GetForwardVector();

	// 박스 콜리전 생성
	FCollisionShape Box = FCollisionShape::MakeBox(ControlledCharacter->ToolMesh->GetStaticMesh()->GetBoundingBox().GetSize() / 2
																				* ControlledCharacter->ToolMesh->GetRelativeScale3D());
	// 박스 이동 방향 결정
	BoxPosition += ForwardDir * Box.GetExtent().X;

	FQuat WorldRotation = ControlledCharacter->ToolMesh->GetComponentQuat();

	
	UE_LOG(LogTemp, Warning, TEXT("WorldRotation at: %s"), *WorldRotation.ToString());

	FTransform BoxT = FTransform(
		//박스의 로컬 회전값을 월드 회전값으로 변환
		WorldRotation,
		BoxPosition,
		ControlledCharacter->ToolMesh->GetRelativeScale3D()
	);
	//FVector BoxPosition = T.GetLocation();
	// 디버그

	// 박스
	// DrawDebugBox(const UWorld* InWorld, FVector const& Center, FVector const& Extent, FColor const& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f) {}
	
	// 라인
	//DrawDebugLine(
	//	GetWorld(),
	//	Start,
	//	Start+BoxPosition,
	//	FColor::Red,   // 색상
	//	false,         // 영구 표시 여부 (false면 일정 시간 후 사라짐)
	//	5.0f,          // 지속 시간 (초)
	//	0,             // 뎁스 우선순위 (디폴트 0)
	//	2.0f           // 선 두께
	//);

	FHitResult HitResult;
	// 스피어의 반지름을 25로 설정 (필요에 따라 값 조정)
	//FCollisionShape Sphere = FCollisionShape::MakeSphere(25.f);

	FCollisionQueryParams	CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	// 스프링암 카메라 위치
	//최종 트레이스 Start = Tool 시작점  Tool의 끝 툴과 동일한 크기의 콜리전 체크 ㄱ 회전은 툴의 Wolrd회전값 가져오기
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult,
		
		BoxT.GetLocation(), BoxT.GetLocation(),

		//박스의 로컬 회전값을 월드 회전값으로 변환
		BoxT.GetRotation(),
		
		ECollisionChannel::ECC_GameTraceChannel1, //충돌 채널 설정
		Box, CollisionQueryParams);
	
	if (HitResult.GetActor() && HitResult.GetActor()->IsA(ABaseSculpture::StaticClass()))
	{
		ABaseSculpture* TargetSculpture = Cast<ABaseSculpture>(HitResult.GetActor());
		UE_LOG(LogTemp, Warning, TEXT("HitSculpture!"));
		//Location
		TargetSculpture->DigSculpture(ControlledCharacter->ToolMesh, BoxT);
	}

	if (bHit)
	{
		DrawDebugBox(GetWorld(),
			BoxPosition,
			Box.GetExtent(),
			WorldRotation,
			FColor::Red, false, 5.0f);

		UE_LOG(LogTemp, Warning, TEXT("Hit at: %s"), *HitResult.ImpactPoint.ToString());
	}
	else
	{
		DrawDebugBox(GetWorld(),
			BoxPosition,
			Box.GetExtent(),
			WorldRotation,
			FColor::Green, false, 5.0f);

		UE_LOG(LogTemp, Warning, TEXT("BoxPosition at: %s"), *BoxPosition.ToString());
		UE_LOG(LogTemp, Warning, TEXT("BoxRotation at: %s"), *WorldRotation.ToString());
	}
}

void ABasePlayerController::OnWheelScroll(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();


	ABasePlayerCharacter* ControlledCharacter = Cast<ABasePlayerCharacter>(GetCharacter());

	FRotator NewRotation = ControlledCharacter->ToolMesh->GetRelativeRotation();
	NewRotation.Roll +=ActionValue.X; // Roll 값만 추가

	ControlledCharacter->SetToolTransform(FTransform
	(NewRotation,
		ControlledCharacter->ToolMesh->GetRelativeLocation(),
		ControlledCharacter->ToolMesh->GetRelativeScale3D()));
}
