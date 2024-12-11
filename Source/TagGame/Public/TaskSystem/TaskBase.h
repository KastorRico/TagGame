// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	ATaskBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	FName TaskID;

	FTaskDetails TaskDetails;

	int32 CurrentStage = 0;

	FStageDetails StageDetails;

	TMap<FString, int32> CurrentObjectiveProgress;

	bool bIsCompleted = false;

public:

	FORCEINLINE void SetTaskID(const FName& NewTaskID) { TaskID = NewTaskID; }
};
