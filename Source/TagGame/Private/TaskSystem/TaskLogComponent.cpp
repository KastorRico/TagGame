// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskSystem/TaskLogComponent.h"

#include "Net/UnrealNetwork.h"
#include "TaskSystem/TaskBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogTaskLogComponent,Log,All)

UTaskLogComponent::UTaskLogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UTaskLogComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTaskLogComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTaskLogComponent, CurrentTasks);
	DOREPLIFETIME(UTaskLogComponent, CurrentActiveTasks);
}

void UTaskLogComponent::AddNewTask(const FName& TaskID)
{
	if (IsTaskActive(TaskID))
	{
		return;
	}

	if (TaskClass == nullptr)
	{
		UE_LOG(LogTaskLogComponent, Warning,TEXT("No Task class is set!"))
		return;
	}
	
	CurrentActiveTasks.Add(TaskID);
	if (ATaskBase* Task = GetWorld()->SpawnActor<ATaskBase>(TaskClass))
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

