#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskSystem.h"
#include "TaskBase.generated.h"

UCLASS()
class TAGGAME_API ATaskBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATaskBase();

protected:
	virtual void BeginPlay() override;

	void GetTaskDetails();

	FObjectiveDetails GetObjectiveDetails(const FString& ObjectiveID);

	UFUNCTION()
	void OnObjectiveIdHeard(const FString& ObjectiveId);



	FName TaskID;
	
	FTaskDetails* TaskDetails;

	int32 CurrentStage = 0;

	FStageDetails StageDetails;

	FString CurrObjectiveID;

	int32 ObjectiveProgress = 0;

	bool bIsCompleted = false;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> TasksTable;

public:

	void SetTaskID(const FName& NewTaskID);
};

