
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TagCharacterAnimInstance.generated.h"

UCLASS()
class TAGGAME_API UTagCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadWrite)
	float Speed;

	UPROPERTY(BlueprintReadWrite)
	float Angle;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsAirborne = false;

};
