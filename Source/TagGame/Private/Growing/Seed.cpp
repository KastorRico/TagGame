// Copyright Oleksandra Pylypiva. All Rights Reserved.


#include "Growing/Seed.h"

ASeed::ASeed()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASeed::Use()
{
	if (--numOfUsages <= 0)
	{
		Destroy();
	}
}

