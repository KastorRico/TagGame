// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TaskLogComponent.generated.h"

class ATaskBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAGGAME_API UTaskLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTaskLogComponent();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<ATaskBase>> CurrentTasks;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> CurrentActiveTasks;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> CompletedTasks;

	UPROPERTY(VisibleAnywhere)
	FName CurrentTask;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddNewTask(const FName& TaskID);

	void CompleteTask();

	void TrackTask();

	bool IsTaskActive(const FName& TaskID);
};
