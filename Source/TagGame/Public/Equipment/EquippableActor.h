// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskSystem/InteractionInterface.h"
#include "EquippableActor.generated.h"

UCLASS()
class TAGGAME_API AEquippableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	AEquippableActor();

protected:

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditDefaultsOnly)
	FString EquippableActorId;

	UPROPERTY(EditDefaultsOnly)
	float ThrowForce = 100.f;

	UPROPERTY(VisibleAnywhere)
	FTransform StartingTransform;

	virtual void BeginPlay() override;

	virtual void LookAt_Implementation() override;

	virtual void StopLookAt_Implementation() override;

	virtual FString Interact_Implementation(ATagGameCharacter* Character) override;

public:

	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() { return StaticMesh; }
	
	void Equip(ACharacter* Character, const FName& AttachSocketName);

	void Unequip();
};
