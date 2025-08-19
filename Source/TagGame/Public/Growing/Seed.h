// Copyright Oleksandra Pylypiva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquippableActor.h"
#include "GameFramework/Actor.h"
#include "Seed.generated.h"

class AGrowableActor;

UCLASS(Abstract)
class TAGGAME_API ASeed : public AEquippableActor
{
	GENERATED_BODY()
	
public:	

	ASeed();

	void Use();

protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGrowableActor> GrowableActorClass;

	UPROPERTY(EditAnywhere)
	int32 numOfUsages = 3;
	
public:

	FORCEINLINE const TSubclassOf<AGrowableActor>& GetGrowableActorClass() const { return GrowableActorClass; }
	
};
