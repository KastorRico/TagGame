#include "TaskSystem/TaskLogComponent.h"

#include "Net/UnrealNetwork.h"
#include "TaskSystem/TaskBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogTaskLogComponent,Log,All)

UTaskLogComponent::UTaskLogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UTaskLogComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UTaskLogComponent, CurrentActiveTasks);
}

void UTaskLogComponent::Client_SpawnTask_Implementation(const FName& TaskID)
{
	if (TaskClass == nullptr)
	{
		UE_LOG(LogTaskLogComponent, Warning, TEXT("Task Class is null"));
	}
	
	if (ATaskBase* Task = GetWorld()->SpawnActor<ATaskBase>(TaskClass))
	{
		Task->SetTaskID(TaskID);
		Task->SetOwner(GetOwner());
		CurrentTasks.Add(Task);
	}
}

void UTaskLogComponent::AddNewTask(const FName& TaskID)
{
	if (IsTaskActive(TaskID))
	{
		return;
	}
	
	CurrentActiveTasks.Add(TaskID);
	Client_SpawnTask(TaskID);
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

