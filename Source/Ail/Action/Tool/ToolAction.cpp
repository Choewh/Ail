// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Tool/ToolAction.h"
#include "Component/ToolChildActorComponent.h"

void UToolAction_Tool::Use()
{
	if (UWorld* World = Context->User->GetWorld())
	{
		APawn* Pawn = Context->User->GetPawn();
		UToolChildActorComponent* Tool = Pawn->GetComponentByClass<UToolChildActorComponent>();
		check(Tool);

		//Pawn 이 가지고있는 ActorComp 를 해당 모양으로 변경

		//AWeaponBase* Weapon = World->SpawnActorDeferred<AWeaponBase>(AWeaponBase::StaticClass(),
		//	FTransform::Identity, Pawn);
		//Weapon->DataTableRowHandle = Handle;
		//
		//USkeletalMeshComponent* SkeletalMeshComponent = Pawn->GetComponentByClass<USkeletalMeshComponent>();
		//check(SkeletalMeshComponent);
		//{
		//	Weapon->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, 
		//		SocketName::Weapon);
		//}
		//Weapon->FinishSpawning(FTransform::Identity);
	}

	Super::Use();
}
