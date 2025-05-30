// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

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
	FString Interact();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool LookAt();
};
