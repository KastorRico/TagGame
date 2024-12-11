// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskSystem/TaskLogComponent.h"

#include "TaskSystem/TaskBase.h"

// Sets default values for this component's properties
UTaskLogComponent::UTaskLogComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTaskLogComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTaskLogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTaskLogComponent::AddNewTask(const FName& TaskID)
{
	if (IsTaskActive(TaskID))
	{
		return;
	}

	CurrentActiveTasks.Add(TaskID);
	if (ATaskBase* Task = GetWorld()->SpawnActor<ATaskBase>())
	{
		Task->SetTaskID(TaskID);
		CurrentTasks.Add(Task);
	}
}

void UTaskLogComponent::CompleteTask()
{
}

void UTaskLogComponent::TrackTask()
{
}

bool UTaskLogComponent::IsTaskActive(const FName& TaskID)
{
	return CurrentActiveTasks.Contains(TaskID);
}

