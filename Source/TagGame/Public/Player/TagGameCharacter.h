#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TagGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTagComponent;
class UTaskLogComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveIdCalled, const FString&);

UCLASS(config=Game)
class ATagGameCharacter : public ACharacter
{
	GENERATED_BODY()
public:

	FOnObjectiveIdCalled OnObjectiveIdCalled;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTagComponent> TagComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTaskLogComponent> TaskLogComponent;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InteractAction;


	UPROPERTY(EditAnywhere)
	float InteractTraceLength = 50.f;

	UPROPERTY(EditAnywhere)
	float InteractTraceRadius = 75.f;

	UPROPERTY(Transient,VisibleAnywhere)
	TObjectPtr<AActor> LookAtActor;

public:
	ATagGameCharacter();
	
protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void InteractTrace();
	void Interact();

	UFUNCTION(Server, Unreliable)
	void Server_Interact();

	UFUNCTION(Client, Unreliable)
	void Client_BroadcastObjectiveIdCalled(const FString& ObjectiveId);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE UTagComponent* GetTagComponent() const { return TagComponent; }

	FORCEINLINE UTaskLogComponent* GetTaskLogComponent() const { return TaskLogComponent; }
};

