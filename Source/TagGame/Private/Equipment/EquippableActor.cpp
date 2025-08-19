// Copyright Oleksandra Pylypiva. All Rights Reserved.


#include "Equipment/EquippableActor.h"

#include "Components/InventoryComponent.h"
#include "GameFramework/Character.h"

AEquippableActor::AEquippableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	StaticMesh->SetupAttachment(Root);
	StaticMesh->SetIsReplicated(true);
	bReplicates = true;
	
	StaticMesh->SetCollisionProfileName(TEXT("Equippable"));
}


void AEquippableActor::BeginPlay()
{
	Super::BeginPlay();

	StartingTransform = StaticMesh->GetRelativeTransform();
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
	Root->AttachToComponent(Character->GetMesh(),
	FAttachmentTransformRules::SnapToTargetNotIncludingScale,AttachSocketName);
	StaticMesh->SetRelativeTransform(StartingTransform);
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


