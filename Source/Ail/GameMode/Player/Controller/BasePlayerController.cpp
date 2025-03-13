// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "GameMode/Player/Character/BasePlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

#include "SculptureSystem/Sculpture.h"
#include "PaintingSystem/DrawSculpture.h"

ABasePlayerController::ABasePlayerController()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/SculptureSystem/Input/IMC_BaseInput.IMC_BaseInput'") };
		check(Asset.Object);

		IMC_Default = Asset.Object;
	}
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
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_LeftClick is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_RightClick")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ABasePlayerController::OnRightClick);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_RightClick is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_MouseWheel")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnWheelScroll);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_MouseWheel is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Test")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnTest);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Test is disabled"));
		//EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABasePlayerController::OnLeftClick);
	}
}
void ABasePlayerController::OnTest(const FInputActionValue& InputActionValue)
{
	ABasePlayerCharacter* ControlledCharacter = Cast<ABasePlayerCharacter>(GetCharacter());

	FVector Start = ControlledCharacter->GetCamera()->GetComponentLocation();
	FVector End = ControlledCharacter->GetCamera()->GetForwardVector() * 300.f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start + End, ECollisionChannel::ECC_GameTraceChannel1, CollisionParams);

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(ASculpture::StaticClass()))
	{
		ASculpture* TargetSculpture = Cast<ASculpture>(HitResult.GetActor());
		UE_LOG(LogTemp, Warning, TEXT("DrawBrush"));
		//Location
		FVector2D UV;
		float BrushSize = 50.f;
		FLinearColor BrushColor = FLinearColor(0, 0, 1, 1);

		UGameplayStatics::FindCollisionUV(HitResult, 0, UV);

		TargetSculpture->ConvertMeshDynamicToStatic();
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
	const FVector UpVector = FVector(0.f, 0.f, 1.f);

	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(UpVector, ActionValue);
}

void ABasePlayerController::OnLeftClick(const FInputActionValue& InputActionValue)
{
	ABasePlayerCharacter* ControlledCharacter = Cast<ABasePlayerCharacter>(GetCharacter());
	//ASculpture Dig Func
	{

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

		// Debuging
		//{
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
		//}

		FHitResult HitResult;

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

		if (HitResult.GetActor() && HitResult.GetActor()->IsA(ASculpture::StaticClass()))
		{
			ASculpture* TargetSculpture = Cast<ASculpture>(HitResult.GetActor());
			UE_LOG(LogTemp, Warning, TEXT("HitSculpture!"));
			//Location
			TargetSculpture->DigSculpture(ControlledCharacter->ToolMesh, BoxT);
		}


		//Debuging
		{
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
	}
}

void ABasePlayerController::OnRightClick(const FInputActionValue& InputActionValue)
{
	ABasePlayerCharacter* ControlledCharacter = Cast<ABasePlayerCharacter>(GetCharacter());
	{
		FVector Start = ControlledCharacter->GetCamera()->GetComponentLocation();
		FVector End = ControlledCharacter->GetCamera()->GetForwardVector() * 300.f;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.bTraceComplex = true;
		CollisionParams.bReturnFaceIndex = true;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start + End, ECollisionChannel::ECC_GameTraceChannel1, CollisionParams);

		if (HitResult.GetActor() && HitResult.GetActor()->IsA(ADrawSculpture::StaticClass()))
		{
			ADrawSculpture* TargetSculpture = Cast<ADrawSculpture>(HitResult.GetActor());
			UE_LOG(LogTemp, Warning, TEXT("DrawBrush"));
			//Location
			if (!TargetSculpture->MeshComponent->GetStaticMesh()
				->GetMeshDescription(0))  // UV 채널이 존재하는지 확인
			{
				UE_LOG(LogTemp, Error, TEXT("LODIndex(0) is NULL"));
				return;
			}

			UStaticMeshComponent* TargetStaticMeshComponent = TargetSculpture->MeshComponent;

			FVector2D UVPosition;
			float BrushSize = 50.f;
			FLinearColor BrushColor = FLinearColor(0, 1, 0, 1);
			//
			if (!TargetStaticMeshComponent)
			{
				UE_LOG(LogTemp, Error, TEXT("TargetStaticMeshComponent is NULL"));
				return;
			}

			
			UBodySetup* BodySetup = TargetStaticMeshComponent->GetBodySetup();
			if (!BodySetup)
			{
				UE_LOG(LogTemp, Error, TEXT("BodySetup is NULL"));
				return;
			}

			// 충돌한 Face Index 확인
			int32 FaceIndex = HitResult.FaceIndex;
			if (FaceIndex < 0)
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid FaceIndex: %d"), FaceIndex);
				return;
			}

			// UV 정보 가져오기
			int32 UVChannel = 1;
			FBodySetupUVInfo FUVInfo;
			FUVInfo = BodySetup->UVInfo;

			// UV 채널 유효성 검사
			if (!FUVInfo.VertUVs.IsValidIndex(UVChannel))
			{
				UE_LOG(LogTemp, Error, TEXT("FUVInfo is Null"));
				return;
			}
			// FaceIndex가 유효한지 검사
			int32 MaxFaceIndex = FUVInfo.IndexBuffer.Num() / 3;
			if (FaceIndex < 0 || FaceIndex >= MaxFaceIndex)
			{
				UE_LOG(LogTemp, Error, TEXT("FaceIndex out of bounds: %d (Max: %d)"), FaceIndex, MaxFaceIndex);
				return;
			}

			// UV 좌표 출력
			UVPosition = FUVInfo.VertUVs[0][UVChannel];
			UE_LOG(LogTemp, Warning, TEXT("UV Coordinates: X = %f, Y = %f"), UVPosition.X, UVPosition.Y);

			
			bool bUV = UGameplayStatics::FindCollisionUV(HitResult, 0, UVPosition);
			if (bUV)
			{
				TargetSculpture->DrawBrush(BrushSize, UVPosition, BrushColor);
			}
		}
	}
}

void ABasePlayerController::OnWheelScroll(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();


	ABasePlayerCharacter* ControlledCharacter = Cast<ABasePlayerCharacter>(GetCharacter());

	FRotator NewRotation = ControlledCharacter->ToolMesh->GetRelativeRotation();
	NewRotation.Roll += ActionValue.X; // Roll 값만 추가

	ControlledCharacter->SetToolTransform(FTransform
	(NewRotation,
		ControlledCharacter->ToolMesh->GetRelativeLocation(),
		ControlledCharacter->ToolMesh->GetRelativeScale3D()));
}