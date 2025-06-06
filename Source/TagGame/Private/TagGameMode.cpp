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
	
	int32 RandomTaskID = FMath::RandRange(0, Tasks.Num()-1);

	TaskLogComponent->AddNewTask(Tasks[RandomTaskID].RowName);
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

