// Copyright Oleksandra Pylypiva. All Rights Reserved.


#include "Growing/GrowableActor.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrowableActor, Log, All)

AGrowableActor::AGrowableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
}


void AGrowableActor::BeginPlay()
{
	Super::BeginPlay();

	if (CurrStage < Meshes.Num())
	{
		StaticMeshComponent->SetStaticMesh(Meshes[CurrStage]);
	}
	
	// Sets up a timer to change stage
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(GrowingTimer,this,
			&AGrowableActor::Grow,TimeToSwitchStage,true);
	}
	
}

void AGrowableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGrowableActor,CurrStage);
}

void AGrowableActor::OnRep_CurrStage()
{
	if (CurrStage >= Meshes.Num()-1)
	{
		bIsReadyToCollect = true;
	}
	
	if (StaticMeshComponent == nullptr)
	{
		UE_LOG(LogGrowableActor,Warning,TEXT("Static Mesh Component is not valid!"));
		return;
	}
	StaticMeshComponent->SetStaticMesh(Meshes[CurrStage]);
}


void AGrowableActor::LookAt_Implementation()
{
	if (StaticMeshComponent == nullptr)
	{
		UE_LOG(LogGrowableActor,Warning,TEXT("Static Mesh Component is not valid!"));
		return;
	}

	StaticMeshComponent->SetOverlayMaterial(OutlineMaterial);
}

void AGrowableActor::StopLookAt_Implementation()
{
	if (StaticMeshComponent == nullptr)
	{
		UE_LOG(LogGrowableActor,Warning,TEXT("Static Mesh Component is not valid!"));
		return;
	}

	StaticMeshComponent->SetOverlayMaterial(nullptr);
}

FString AGrowableActor::Interact_Implementation(ATagGameCharacter* Character)
{
	if(!bIsReadyToCollect)
	{
		UE_LOG(LogGrowableActor,Warning,TEXT("Is not ready to collect!"));
		return "";
	}
	Destroy();
	return ObjectiveID;
}

void AGrowableActor::Grow()
{
	if (++CurrStage >= Meshes.Num()-1)
	{
		bIsReadyToCollect = true;
		GetWorld()->GetTimerManager().ClearTimer(GrowingTimer);
	}
	
	if (StaticMeshComponent == nullptr)
	{
		UE_LOG(LogGrowableActor,Warning,TEXT("Static Mesh Component is not valid!"));
		return;
	}

	StaticMeshComponent->SetStaticMesh(Meshes[CurrStage]);
}

void AGrowableActor::PauseGrowingTimer(bool Value)
{
	if (Value)
	{
		GetWorld()->GetTimerManager().PauseTimer(GrowingTimer);
	}
	else
	{
		GetWorld()->GetTimerManager().UnPauseTimer(GrowingTimer);
	}
}



