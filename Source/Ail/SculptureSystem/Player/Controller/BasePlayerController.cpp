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
		UE_LOG(LogTemp, Warning, TEXT("IA_Up is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_LeftClick")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ABasePlayerController::OnLeftClick);
		//EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnLeftClick);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Up is disabled"));
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

	FVector Start = ControlledCharacter->GetCamera()-> GetComponentLocation();
	FVector End = ControlledCharacter->GetCamera()->GetForwardVector()*300.f;
	

	//박스용
	//FVector HalfSize = FVector(10.f, 10.f, 10.f);
	//FRotator BoxRotator = FRotator::ZeroRotator;
	//TArray<AActor*> IgnoreActors;
	
	// 디버그
	// 
	// 박스
	// DrawDebugBox(const UWorld* InWorld, FVector const& Center, FVector const& Extent, FColor const& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f) {}
	
	// 라인
	//DrawDebugLine(
	//	GetWorld(),
	//	Start,
	//	Start+End,
	//	FColor::Red,   // 색상
	//	false,         // 영구 표시 여부 (false면 일정 시간 후 사라짐)
	//	5.0f,          // 지속 시간 (초)
	//	0,             // 뎁스 우선순위 (디폴트 0)
	//	2.0f           // 선 두께
	//);
	UE_LOG(LogTemp, Warning, TEXT("Camera Vec %s"), *Start.ToString());
	UE_LOG(LogTemp, Warning, TEXT("End Vec %s"), *(Start+End).ToString());

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	//@TODO 
	//라인 트레이스 -> 툴따라가기 스프링암 길이
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start + End, ECollisionChannel::ECC_GameTraceChannel1, CollisionParams);
	
	FVector UnfixedCameraPosition = ControlledCharacter->SpringArm->GetUnfixedCameraPosition();

	//if (HitResult.GetActor() && HitResult.GetActor()->IsA(ABaseSculpture::StaticClass()))
	//{
	//	ABaseSculpture* TargetSculpture = Cast<ABaseSculpture>(HitResult.GetActor());
	//	UE_LOG(LogTemp, Warning, TEXT("HitSculpture!"));
	//	//Location
	//	TargetSculpture->DigSculpture(HitResult.Location, ControlledCharacter->GetCamera()->GetComponentRotation());
	//}
}
