// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskSystem/TaskBase.h"

// Sets default values
ATaskBase::ATaskBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATaskBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATaskBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

