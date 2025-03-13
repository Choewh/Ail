// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "GameMode/Player/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

ABaseHUD::ABaseHUD()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Tool/Input/IMC_Tool.IMC_Tool'") };
		check(Asset.Object);

		IMC_Tool = Asset.Object;
	}
}

void ABaseHUD::AddDroppedTool(ADroppedTool* NewTool)
{
	Widget->AddDroppedTool(NewTool);
}

void ABaseHUD::RemoveDroppedTool(ADroppedTool* NewTool)
{
	Widget->RemoveDroppedTool(NewTool);
}

void ABaseHUD::OnPickupTool(const FInputActionValue& InputActionValue)
{
	if (OverlappedDroppedTools.IsEmpty()) { return; }

	ADroppedTool* DroppedTool = static_cast<ADroppedTool*>(*OverlappedDroppedTools.begin());
	DroppedTool->UseTool(PlayerController);
	DroppedTool->Destroy();
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	UClass* WidgetClass = LoadClass<UToolWidget>(nullptr,
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/Widget/DroppedTool.DroppedTool_C'"));
	check(WidgetClass);
	Widget = CreateWidget<UToolWidget>(GetWorld(), WidgetClass);
	Widget->AddToViewport();

	PlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	check(PlayerController);

	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		Subsystem->AddMappingContext(IMC_Tool, 0);
		EnableInput(PlayerController);
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		ensure(EnhancedInputComponent);
		if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Tool, TEXT("IA_PickupTool")))
		{
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnPickupTool);		}
	}
}

void ABaseHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (APawn* Pawn = PlayerController->GetPawn())
	{
		TArray<AActor*> Temp;
		Pawn->GetOverlappingActors(Temp, ADroppedTool::StaticClass());

		if (Temp != OverlappedDroppedTools)
		{
			for (AActor* It : OverlappedDroppedTools)
			{
				RemoveDroppedTool(static_cast<ADroppedTool*>(It));
			}
			OverlappedDroppedTools = MoveTemp(Temp);

			for (AActor* It : OverlappedDroppedTools)
			{
				AddDroppedTool(static_cast<ADroppedTool*>(It));
			}
		}
	}
}


