// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskSystem/InteractionInterface.h"
#include "GrowingBed.generated.h"

class AGrowableActor;

UCLASS(Abstract)
class TAGGAME_API AGrowingBed : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AGrowingBed();

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	virtual void BeginPlay() override;

	virtual void LookAt_Implementation() override;

	virtual void StopLookAt_Implementation() override;

	virtual FString Interact_Implementation(ATagGameCharacter* Character) override;

	UFUNCTION()
	void Collect(AActor* CollectedActor);

	UFUNCTION()
	void PauseGrowing();

	void UnpauseGrowing();

	UPROPERTY(EditDefaultsOnly)
	FString ObjectiveID {"GrowingBed"};

	UPROPERTY(EditAnywhere)
	bool bIsSeedNeeded = false;

	UPROPERTY(EditAnywhere)
	bool bCanBeDry = true;

	FTimerHandle DryingTimer;

	UPROPERTY(EditAnywhere)
	float TimeToBeWatered = 8.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGrowableActor> GrowableActorClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> DryMaterial;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInterface> WetMaterial;

	UPROPERTY(Transient)
	TObjectPtr<AGrowableActor> GrowableActor;

	UPROPERTY(EditAnywhere)
	int32 Stage = 0;

private:

	void SetDryingTimer();

	bool bIsDry = false;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ChangeMaterial(UMaterialInterface* Material);

};


