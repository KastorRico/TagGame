// Copyright Oleksandra Pylypiva. All Rights Reserved.


#include "Growing/GrowingBed.h"

#include "Components/InventoryComponent.h"
#include "Equipment/WateringActor.h"
#include "Growing/GrowableActor.h"
#include "Growing/Seed.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrowingBedActor, Log, All)

AGrowingBed::AGrowingBed()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetIsReplicated(true);
	bReplicates = true;
}

void AGrowingBed::BeginPlay()
{
	Super::BeginPlay();

	WetMaterial = StaticMesh->GetMaterial(0);
	
	SetDryingTimer();
	
	if (bIsSeedNeeded)
	{
		GrowableActorClass = nullptr;
		return;
	}

	if(GrowableActorClass == nullptr)
	{
		UE_LOG(LogGrowingBedActor, Warning, TEXT("GrowableActorClass is null"));
		return;
	}
	
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
	GrowableActor = GetWorld()->SpawnActorDeferred<AGrowableActor>(GrowableActorClass,
		GetActorTransform());
	if(GrowableActor != nullptr)
	{
		GrowableActor->SetCurStage(Stage);
		GrowableActor->FinishSpawning(FTransform(GetActorLocation()));
		GrowableActor->OnDestroyed.AddDynamic(this,&AGrowingBed::Collect);
	}
	
}


void AGrowingBed::LookAt_Implementation()
{
	
}

void AGrowingBed::StopLookAt_Implementation()
{
	
}

FString AGrowingBed::Interact_Implementation(ATagGameCharacter* Character)
{
	UInventoryComponent* Inventory = Character->GetInventoryComponent();
	check(Inventory != nullptr);

	if(AWateringActor* WateringActor = Cast<AWateringActor>(Inventory->GetActiveItem()))
	{
		UnpauseGrowing();
		return "Watered";
	}
	if(bIsDry)
	{
		return FString();
	}
	
	if(ASeed* Seed = Cast<ASeed>(Inventory->GetActiveItem()))
	{
		UE_LOG(LogGrowingBedActor, Warning, TEXT("USING SEEEEDS"));
		GrowableActorClass = Seed->GetGrowableActorClass();
		Seed->Use();
	}
	if(GrowableActorClass == nullptr)
	{
		UE_LOG(LogGrowingBedActor, Warning, TEXT("GrowableActorClass is null"));
		return FString();
	}

	GrowableActor = GetWorld()->SpawnActor<AGrowableActor>(GrowableActorClass,
		FTransform(GetActorLocation()));
	if(GrowableActor == nullptr)
	{
		return FString();
	}
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
	GrowableActor->OnDestroyed.AddDynamic(this,&AGrowingBed::Collect);
	return ObjectiveID;
}

void AGrowingBed::Collect(AActor* CollectedActor)
{
	UE_LOG(LogGrowingBedActor, Warning, TEXT("GrowableActor is collected!"));
	GrowableActorClass = nullptr;
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Block);
}

void AGrowingBed::PauseGrowing()
{
	Multicast_ChangeMaterial(DryMaterial);
	bIsDry = true;
	if(GrowableActor == nullptr)
	{
		return;
	}
	GrowableActor->GetStaticMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Block);
	GrowableActor->PauseGrowingTimer(true);
}

void AGrowingBed::UnpauseGrowing()
{
	Multicast_ChangeMaterial(WetMaterial);
	bIsDry = false;
	SetDryingTimer();
	
	if(GrowableActor == nullptr)
	{
		return;
	}
	GrowableActor->GetStaticMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
	GrowableActor->PauseGrowingTimer(false);
}

void AGrowingBed::SetDryingTimer()
{
	if (bCanBeDry && HasAuthority())
	{
		UE_LOG(LogGrowingBedActor, Warning, TEXT("Setting drying timer!"));
		GetWorld()->GetTimerManager().SetTimer(DryingTimer,this,
			&AGrowingBed::PauseGrowing,TimeToBeWatered);
	}
}

void AGrowingBed::Multicast_ChangeMaterial_Implementation(UMaterialInterface* Material)
{
	StaticMesh->SetMaterial(0,Material);
}
