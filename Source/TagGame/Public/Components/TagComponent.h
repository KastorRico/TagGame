// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TagComponent.generated.h"

class ATagGameCharacter;
class UInputAction;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAGGAME_API UTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTagComponent();

protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void Tag();

	UFUNCTION()
	virtual void SetupInput();

	UPROPERTY(EditAnywhere)
	float TagTraceLength = 50.f;

	UPROPERTY(EditAnywhere)
	float TagTraceRadius = 25.f;

	UPROPERTY(Replicated,VisibleAnywhere)
	bool bIsChaser = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> TagAction;

	UPROPERTY(Transient)
	ATagGameCharacter* OwnerCharacter;

public:

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTagCharacter(ATagGameCharacter* NewChaser);

	FORCEINLINE void SetIsChaser(bool Value) { bIsChaser = Value; }
};
