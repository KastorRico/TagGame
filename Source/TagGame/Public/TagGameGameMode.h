// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TagGameGameMode.generated.h"

UCLASS(minimalapi)
class ATagGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATagGameGameMode();

protected:

	void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;

};



