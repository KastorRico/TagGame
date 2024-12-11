// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskSystem.generated.h"

UENUM(BlueprintType)
enum class EObjectiveType : uint8 {
	Location       UMETA(DisplayName = "Location"),
	Interact       UMETA(DisplayName = "Interact"),
	Collect        UMETA(DisplayName = "Collect"),
};

USTRUCT(BlueprintType)
struct FObjectiveDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectiveType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ObjectiveLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	bool bIsOptional = false;
};

USTRUCT(BlueprintType)
struct FStageDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FObjectiveDetails Objective;

};

USTRUCT(BlueprintType)
struct FTaskDetails : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStageDetails> Stages;
};