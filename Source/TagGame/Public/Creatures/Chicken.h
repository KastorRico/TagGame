// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TaskSystem/InteractionInterface.h"
#include "Chicken.generated.h"

UCLASS()
class TAGGAME_API AChicken : public ACharacter,  public IInteractionInterface
{
	GENERATED_BODY()

public:

	AChicken();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
