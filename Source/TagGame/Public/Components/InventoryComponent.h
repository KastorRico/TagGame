// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AEquippableActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAGGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnActiveItemDestroyed(AActor* Actor);

	UPROPERTY(EditDefaultsOnly)
	FName ActiveItemSocket;

	UPROPERTY(Replicated, VisibleAnywhere)
	TObjectPtr<AEquippableActor> ActiveItem;

public:

	void SetActiveItem(TObjectPtr<AEquippableActor> Item);

	UFUNCTION(Server, Reliable)
	void Server_DropItem();
	
	FORCEINLINE AEquippableActor* GetActiveItem() { return ActiveItem; }
	
};

