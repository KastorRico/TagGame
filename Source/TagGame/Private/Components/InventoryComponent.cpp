// Copyright Oleksandra Pylypiva. All Rights Reserved.

#include "Components/InventoryComponent.h"

#include "Equipment/EquippableActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent,ActiveItem);
}

void UInventoryComponent::OnActiveItemDestroyed(AActor* Actor)
{
	ActiveItem = nullptr;
}

// Sets an Item as an active item if a player already has an active items they will drop it
void UInventoryComponent::SetActiveItem(TObjectPtr<AEquippableActor> Item)
{
	if(Item == nullptr || Item == ActiveItem)
	{
		return;
	}
	
	Server_DropItem();
	ActiveItem = Item;
	ActiveItem->OnDestroyed.AddUniqueDynamic(this,&UInventoryComponent::OnActiveItemDestroyed);
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	check(Character != nullptr);
	ActiveItem->Equip(Character,ActiveItemSocket);
}

void UInventoryComponent::Server_DropItem_Implementation()
{
	if (ActiveItem == nullptr)
	{
		return;
	}
	
	ActiveItem->OnDestroyed.RemoveDynamic(this,&UInventoryComponent::OnActiveItemDestroyed);
	ActiveItem->Unequip();
	ActiveItem = nullptr;
}
