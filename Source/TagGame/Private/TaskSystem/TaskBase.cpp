#include "TaskSystem/TaskBase.h"

#include "Player/TagGameCharacter.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogTaskBase,Log,All)

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
		UE_LOG(LogTaskBase,Error,TEXT("TasksTable is not valid!"));
		return;
	}

	TaskDetails = TasksTable->FindRow<FTaskDetails>(TaskID,"");
	if (TaskDetails == nullptr)
	{
		UE_LOG(LogTaskBase,Error,TEXT("TaskDetails is not valid!"));
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
	
	if (CurrObjectiveID ==  ObjectiveId)
	{
		if (ObjectiveProgress < GetObjectiveDetails(ObjectiveId).Quantity)
		{
			ObjectiveProgress++;
		}
	}
}

void ATaskBase::SetTaskID(const FName& NewTaskID)
{
	TaskID = NewTaskID;
	GetTaskDetails();
}
