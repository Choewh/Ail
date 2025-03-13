// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ToolChildActorComponent.h"
#include "Tool/ToolBase.h"

UToolChildActorComponent::UToolChildActorComponent()
{
#if WITH_EDITOR
	SetChildActorClass(AToolBase::StaticClass());
#endif
}

void UToolChildActorComponent::SetData()
{
	//기존의 액터와 들어온 액터의 정보를 대조후 다르다면 변경
	//겟차일드액터 -> AToolBase 의 정보를 수정
}
