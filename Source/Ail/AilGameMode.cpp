// Copyright Epic Games, Inc. All Rights Reserved.

#include "AilGameMode.h"
#include "AilCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameMode/HUD/BaseHUD.h"
#include "GameMode/Player/Controller/BasePlayerController.h"

AAilGameMode::AAilGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/SculptureSystem/BP_BasePlayer.BP_BasePlayer'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
//		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass =	ABasePlayerController::StaticClass();

	HUDClass = ABaseHUD::StaticClass();
}
