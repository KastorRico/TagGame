// Copyright Oleksandra Pylypiva. All Rights Reserved.


#include "Equipment/EquippableActor.h"

#include "Components/InventoryComponent.h"
#include "GameFramework/Character.h"

AEquippableActor::AEquippableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	bReplicates = true;
	StaticMesh->SetIsReplicated(true);
	StaticMesh->SetCollisionProfileName(TEXT("Equippable"));
}

void AEquippableActor::LookAt_Implementation()
{
	
}

void AEquippableActor::StopLookAt_Implementation()
{

}

FString AEquippableActor::Interact_Implementation(ATagGameCharacter* Character)
{
	if(Character == nullptr)
	{
		return FString();
	}
	if(UInventoryComponent* InventoryComponent =  Character->GetInventoryComponent())
	{
		InventoryComponent->SetActiveItem(this);
	}
	return EquippableActorId;
}

void AEquippableActor::Equip(ACharacter* Character, const FName& AttachSocketName)
{
	if(StaticMesh == nullptr)
	{
		return;
	}
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(Character->GetMesh(),
	FAttachmentTransformRules::SnapToTargetNotIncludingScale,AttachSocketName);
	StaticMesh->SetRelativeTransform(AttachmentTransform);
	SetOwner(Character);
}

void AEquippableActor::Unequip()
{
	if(StaticMesh == nullptr)
	{
		return;
	}
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if(AActor* Character = GetOwner())
	{
		StaticMesh->AddVelocityChangeImpulseAtLocation(Character->GetActorForwardVector()*ThrowForce,
													   GetActorLocation());
	}
	SetOwner(nullptr);
}

void AEquippableActor::UseOnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(OnUsageMontageEnded.IsBound())
	{
		OnUsageMontageEnded.Broadcast();
		OnUsageMontageEnded.Clear();
	}
}

void AEquippableActor::PlayMontageAndUse_Implementation(ACharacter* Player)
{
	if (UsageAnimMontage == nullptr)
	{
		return;
	}
	if (Player == nullptr)
	{
		return;
	}
	check(Player->GetMesh() != nullptr)
	if (UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance())
	{
		Player->PlayAnimMontage(UsageAnimMontage);
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this,&AEquippableActor::UseOnMontageEnded);
	}
}


