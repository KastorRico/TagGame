// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskSystem/InteractionInterface.h"
#include "GrowableActor.generated.h"

UCLASS()
class TAGGAME_API AGrowableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	AGrowableActor();

protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_CurrStage();
	
	virtual void LookAt_Implementation() override;

	virtual void StopLookAt_Implementation() override;

	virtual FString Interact_Implementation() override;

	UFUNCTION()
	void Grow();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> OutlineMaterial;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumOfStages = 3;

	UPROPERTY(ReplicatedUsing=OnRep_CurrStage,EditAnywhere)
	int32 CurrStage = 0;

	UPROPERTY(EditDefaultsOnly)
	TArray<UStaticMesh*> Meshes;

	UPROPERTY(EditDefaultsOnly)
	float TimeToSwitchStage = 1.0f;

	FTimerHandle GrowingTimer;
	
	bool bIsReadyToCollect = false;
};
