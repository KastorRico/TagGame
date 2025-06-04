#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TagComponent.generated.h"

class ATagGameCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAGGAME_API UTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTagComponent();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated,VisibleAnywhere)
	bool bIsChaser = false;

public:

	UFUNCTION()
	void Tag(ATagGameCharacter* TaggedActor);

	FORCEINLINE void SetIsChaser(bool Value) { bIsChaser = Value; }
	FORCEINLINE bool IsChaser() const { return bIsChaser; }
};
