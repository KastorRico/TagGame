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
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void Client_SpawnTask(const FName& TaskID);
	
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCurrentTasks)
	TArray<TObjectPtr<ATaskBase>> CurrentTasks;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintGetter=GetCurrentActiveTasks)
	TArray<FName> CurrentActiveTasks;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> CompletedTasks;

	UPROPERTY(VisibleAnywhere)
	FName CurrentTask;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATaskBase> TaskClass;

public:	
	
	void AddNewTask(const FName& TaskID);

	void CompleteTask();

	void TrackTask();

	bool IsTaskActive(const FName& TaskID);

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<FName> GetCurrentActiveTasks() const { return CurrentActiveTasks; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<ATaskBase*> GetCurrentTasks() const {return CurrentTasks;}
};
