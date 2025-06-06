// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TagGameMode.generated.h"

class ATagGameCharacter;

UCLASS(minimalapi)
class ATagGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATagGameMode();

protected:
	virtual void BeginPlay() override;

	void GiveTasks(ATagGameCharacter* TagCharacter);

	UFUNCTION()
	void GiveTasksToNewPawn(APawn* OldPawn, APawn* NewPawn);

	virtual void OnPostLogin(AController* NewPlayer) override;

	UPROPERTY(EditDefaultsOnly,meta = (DataTable = "/Game/DT_TaskDetails"))
	TArray<FDataTableRowHandle> Tasks;
	

};



