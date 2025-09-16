// Copyright Oleksandra Pylypiva. All Rights Reserved.


#include "Equipment/WateringActor.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

void AWateringActor::BeginPlay()
{
	Super::BeginPlay();

	if (WateringParticle == nullptr)
	{
		return;
	}
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(WateringParticle,
	StaticMesh, NAME_None, ParticlesPosition, FRotator(0.f),
	EAttachLocation::Type::KeepRelativeOffset, false,false);
}

void AWateringActor::UseOnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::UseOnMontageEnded(Montage, bInterrupted);

	if(NiagaraComponent != nullptr)
	{
		NiagaraComponent->Deactivate();	
	}
}


void AWateringActor::PlayMontageAndUse_Implementation(ACharacter* Player)
{
	Super::PlayMontageAndUse_Implementation(Player);

	if (WateringParticle == nullptr || NiagaraComponent == nullptr)
	{
		return;
	}
	NiagaraComponent->Activate();
}
