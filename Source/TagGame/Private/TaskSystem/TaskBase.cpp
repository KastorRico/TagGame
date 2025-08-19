#include "TaskSystem/TaskBase.h"

#include "Player/TagGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogTaskBase, Log, All)

ATaskBase::ATaskBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATaskBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (ATagGameCharacter* TagGameCharacter =
		Cast<ATagGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
	{
		TagGameCharacter->OnObjectiveIdCalled.AddUObject(this, &ATaskBase::OnObjectiveIdHeard);
	}
	
}

void ATaskBase::GetTaskDetails()
{
	if(TasksTable == nullptr)
	{
		UE_LOG(LogTaskBase,Warning,TEXT("TasksTable is not valid!"));
		return;
	}

	TaskDetails = TasksTable->FindRow<FTaskDetails>(TaskID,"");
	if (TaskDetails == nullptr)
	{
		UE_LOG(LogTaskBase,Warning,TEXT("TaskDetails is not valid!"));
		return;
	}
	if (CurrentStage >= TaskDetails->Stages.Num())
	{
		return;
	}

	StageDetails = TaskDetails->Stages[CurrentStage];
	CurrObjectiveID = StageDetails.Objective.ObjectiveID;
	ObjectiveProgress = 0;
}

FObjectiveDetails ATaskBase::GetObjectiveDetails(const FString& ObjectiveID)
{
	if (StageDetails.Objective.ObjectiveID == ObjectiveID)
	{
		return StageDetails.Objective;
	}

	return FObjectiveDetails();
}

void ATaskBase::OnObjectiveIdHeard(const FString& ObjectiveId)
{
	if (CurrObjectiveID != ObjectiveId)
	{
		return;
	}
	
	if (ObjectiveProgress < GetObjectiveDetails(ObjectiveId).Quantity)
	{
		ObjectiveProgress++;
		OnObjectiveProgressChanged.Broadcast();
		if(IsObjectiveCompleted(ObjectiveId))
		{
			// Checks if all stages are completed
			if(++CurrentStage >= TaskDetails->Stages.Num())
			{
				bIsCompleted = true;
				OnTaskCompleted.Broadcast();
				return;
			}
			
			//  Changes the stage of the task
			StageDetails = TaskDetails->Stages[CurrentStage];
			CurrObjectiveID = StageDetails.Objective.ObjectiveID;
			ObjectiveProgress = 0;
			OnStageChanged.Broadcast();
		}
	}
	
}

void ATaskBase::SetTaskID(const FName& NewTaskID)
{
	TaskID = NewTaskID;
	GetTaskDetails();
}

bool ATaskBase::IsObjectiveCompleted(const FString& ObjectiveID)
{
	if(GetObjectiveDetails(ObjectiveID).Quantity <= ObjectiveProgress )
	{
		return true;
	}
	return false;
}
