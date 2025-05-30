// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameMode.h"
#include "TagGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Components/TagComponent.h"
#include "TaskSystem/TaskLogComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTagGameMode,Log,All)
ATagGameMode::ATagGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATagGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	int32 RandomPlayerIndex = FMath::RandRange(0, 0);
	APlayerState* RandomPlayerState = GameState->PlayerArray[RandomPlayerIndex];
	ATagGameCharacter* TagCharacter = Cast<ATagGameCharacter>(RandomPlayerState->GetPawn());
	if (TagCharacter == nullptr)
	{
		return;
	}

	if (UTagComponent* TagComponent = TagCharacter->GetTagComponent())
	{
		TagComponent->SetIsChaser(true);
	}

	GiveTasks();
}

void ATagGameMode::GiveTasks()
{
	if (Tasks.Num() == 0)
	{
		UE_LOG(LogTagGameMode, Warning, TEXT("EMPTY TASKS"));
		return;
	};
	for(APlayerState* PlayerState:GameState->PlayerArray)
	{
		ATagGameCharacter* TagCharacter = Cast<ATagGameCharacter>(PlayerState->GetPawn());
		if (TagCharacter == nullptr)
		{
			return;
		}
		UTaskLogComponent* TaskLogComponent =  TagCharacter->GetTaskLogComponent();
		if(TaskLogComponent == nullptr)
		{
			return;
		}
		
		int32 RandomTaskID = FMath::RandRange(0, Tasks.Num()-1);

		TaskLogComponent->AddNewTask(Tasks[RandomTaskID].RowName);
	}
}

void ATagGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
}

