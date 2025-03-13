// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/Tool/ToolAction.h"
#include "Component/ToolChildActorComponent.h"

void UToolction_Weapon::Use()
{
	if (UWorld* World = Context->User->GetWorld())
	{
		APawn* Pawn = Context->User->GetPawn();
		UToolChildActorComponent* Weapon = Pawn->GetComponentByClass<UToolChildActorComponent>();
		check(Weapon);

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
