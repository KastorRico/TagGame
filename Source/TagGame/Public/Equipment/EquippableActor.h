// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskSystem/InteractionInterface.h"
#include "EquippableActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnUsageMontageEnded);

UCLASS()
class TAGGAME_API AEquippableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	AEquippableActor();
	
protected:
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="Objective")
	FString EquippableActorId;

	UPROPERTY(EditDefaultsOnly, Category="EquippableActor")
	float ThrowForce = 100.f;

	UPROPERTY(EditDefaultsOnly, Category="EquippableActor")
	TObjectPtr<UAnimMontage> UsageAnimMontage;

	UPROPERTY(EditAnywhere, Category="EquippableActor")
	FTransform AttachmentTransform;

	virtual void LookAt_Implementation() override;

	virtual void StopLookAt_Implementation() override;

	virtual FString Interact_Implementation(ATagGameCharacter* Character) override;

	UFUNCTION()
	virtual void UseOnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:

	FOnUsageMontageEnded OnUsageMontageEnded;
	
	void Equip(ACharacter* Character, const FName& AttachSocketName);

	void Unequip();

	UFUNCTION(NetMulticast, Unreliable)
	virtual void PlayMontageAndUse(ACharacter* Player);

	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() { return StaticMesh; }
	
};
