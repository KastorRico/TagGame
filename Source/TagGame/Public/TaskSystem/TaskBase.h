#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskSystem.h"
#include "TaskBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProgressChanged);

UCLASS()
class TAGGAME_API ATaskBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATaskBase();

	UPROPERTY(BlueprintAssignable,Category="Task")
	FOnProgressChanged OnObjectiveProgressChanged;

	FObjectiveDetails GetObjectiveDetails(const FString& ObjectiveID);
	
protected:
	virtual void BeginPlay() override;
	
	void GetTaskDetails();

	UFUNCTION()
	void OnObjectiveIdHeard(const FString& ObjectiveId);
	
	FName TaskID;
	
	FTaskDetails* TaskDetails;
	
	int32 CurrentStage = 0;
	
	FStageDetails StageDetails;

	UPROPERTY(BlueprintReadOnly)
	FString CurrObjectiveID;

	UPROPERTY(BlueprintReadOnly)
	int32 ObjectiveProgress = 0;

	bool bIsCompleted = false;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> TasksTable;

public:

	void SetTaskID(const FName& NewTaskID);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetTaskID() const { return TaskID; }
};


