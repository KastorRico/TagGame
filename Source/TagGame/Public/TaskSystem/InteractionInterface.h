// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/TagGameCharacter.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class ATagGameCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TAGGAME_API IInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString Interact(ATagGameCharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LookAt();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopLookAt();
};
