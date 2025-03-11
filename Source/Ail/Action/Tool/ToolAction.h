// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ToolAction.generated.h"

/**
 * 
 */
UCLASS()
class AIL_API UToolAction : public UObject
{
	GENERATED_BODY()
	
public:
	struct FToolContext
	{
		// Tool을 사용하는 주체
		APlayerController* User;
	};
	using FContext = FToolContext;

public:
	void SetContext(TUniquePtr<FContext> NewContext) { check(!Context); Context = MoveTemp(NewContext); }

public:
	virtual void Use() { ReleaseContext(); }
	virtual void Equip() { Use(); }
	virtual void Unequip() { ReleaseContext(); }

private:
	void ReleaseContext() { check(Context); Context = nullptr; }

protected:
	TUniquePtr<FContext> Context;
};

UCLASS()
class AIL_API UToolction_Weapon : public UToolAction
{
	GENERATED_BODY()
public:
	virtual void Use();
	virtual void Equip() { Use(); }
};
