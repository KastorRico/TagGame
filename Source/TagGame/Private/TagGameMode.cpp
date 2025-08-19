// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameMode.h"
#include "Player/TagGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Components/TagComponent.h"
#include "TaskSystem/TaskLogComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTagGameMode,Log,All)
ATagGameMode::ATagGameMode()
{

}

void ATagGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	int32 RandomPlayerIndex = FMath::RandRange(0, 0);
	APlayerState* RandomPlayerState = GameState->PlayerArray[RandomPlayerIndex];
	ATagGameCharacter* TagCharacter = Cast<ATagGameCharacter>(RandomPlayerState->GetPawn());
	if (TagCharacter == nullptr)
	{
		UE_LOG(LogTagGameMode,Error,TEXT("Attempting to get a null Pawn"));
		return;
	}

	if (UTagComponent* TagComponent = TagCharacter->GetTagComponent())
	{
		TagComponent->SetIsChaser(true);
	}
}

void ATagGameMode::GiveTasks(ATagGameCharacter* TagCharacter)
{
	if (TagCharacter == nullptr)
	{
		UE_LOG(LogTagGameMode,Warning,TEXT("Character is not valid!"));
		return;
	}
	
	UTaskLogComponent* TaskLogComponent = TagCharacter->GetTaskLogComponent();
	if(TaskLogComponent == nullptr)
	{
		UE_LOG(LogTagGameMode,Error,TEXT("TaskLogComponent is not valid!"));
		return;
	}
	
	ShuffleTasks();
	
	for(int32 i = 0; i < Tasks.Num() && i<NumOfTasks; ++i)
	{
		TaskLogComponent->AddNewTask(Tasks[i].RowName);	
	}
}

void ATagGameMode::GiveTasksToNewPawn(APawn* OldPawn, APawn* NewPawn)
{
	if (NewPawn == nullptr)
	{
		return;
	}

	if (ATagGameCharacter* ATagCharacter = Cast<ATagGameCharacter>(NewPawn))
	{
		GiveTasks(ATagCharacter);
	}
}

void ATagGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	NewPlayer->OnPossessedPawnChanged.AddUniqueDynamic(this,&ATagGameMode::GiveTasksToNewPawn);
}

void ATagGameMode::ShuffleTasks()
{
	for(int32 i=0;i<Tasks.Num() && i<NumOfTasks;++i)
	{
		int32 Index = FMath::RandRange(i, Tasks.Num()-1);

		if(i != Index)
		{
			Tasks.Swap(i,Index);
		}
	}
}

