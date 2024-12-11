// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameGameMode.h"
#include "TagGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "TagGameCharacter.h"
#include "Components/TagComponent.h"
#include "TaskSystem/TaskLogComponent.h"

ATagGameGameMode::ATagGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATagGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	int32 RandomPlayerIndex = FMath::RandRange(0, GameState->PlayerArray.Num()-1);
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

void ATagGameGameMode::GiveTasks()
{
	if (Tasks.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EMPTY TASKS"));
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

void ATagGameGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
}

