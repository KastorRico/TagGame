// Copyright Oleksandra Pylypiva. All Rights Reserved.


#include "Creatures/Chicken.h"

// Sets default values
AChicken::AChicken()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChicken::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChicken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChicken::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

