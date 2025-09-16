// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquippableActor.h"
#include "WateringActor.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS(Abstract)
class TAGGAME_API AWateringActor : public AEquippableActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = Watering)
	TObjectPtr<UNiagaraSystem> WateringParticle;

	UPROPERTY(EditDefaultsOnly, Category = Watering)
	FVector ParticlesPosition;

	UPROPERTY(Transient)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	virtual void BeginPlay() override;
	
	virtual void UseOnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

public:

	virtual void PlayMontageAndUse_Implementation(ACharacter* Player) override;
};
