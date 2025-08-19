#include "Player/TagGameCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/InventoryComponent.h"
#include "Components/TagComponent.h"
#include "TaskSystem/TaskLogComponent.h"
#include "TaskSystem/InteractionInterface.h"
#include "Equipment/EquippableActor.h"

DEFINE_LOG_CATEGORY(LogTagCharacter);

//////////////////////////////////////////////////////////////////////////
// ATagGameCharacter

ATagGameCharacter::ATagGameCharacter()
{
	bReplicates = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 

	TagComponent = CreateDefaultSubobject<UTagComponent>(TEXT("TagComponent"));
	TaskLogComponent = CreateDefaultSubobject<UTaskLogComponent>(TEXT("TaskLogComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ATagGameCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ATagGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractTrace();
}

void ATagGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATagGameCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATagGameCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction,
			ETriggerEvent::Started,this,&ATagGameCharacter::Interact);
		
		EnhancedInputComponent->BindAction(DropItemAction,
			ETriggerEvent::Started,this,&ATagGameCharacter::DropActiveItem);
	}
	else
	{
		UE_LOG(LogTagCharacter, Error, TEXT("Failed to find an Enhanced Input component!"));
	}
	
}

void ATagGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATagGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATagGameCharacter::InteractTrace()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * InteractTraceLength;
	DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,3.f);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	GetWorld()->SweepSingleByChannel(HitResult,
	                                    Start,End,FQuat(),ECC_GameTraceChannel1,
	                                    FCollisionShape::MakeSphere(InteractTraceRadius),
	                                    CollisionQueryParams);
	
	
	if (!HitResult.bBlockingHit)
	{
		if (Cast<IInteractionInterface>(LookAtActor) && IsLocallyControlled())
		{
			IInteractionInterface::Execute_StopLookAt(LookAtActor);
		}
		LookAtActor = nullptr;
		return ;
	}
	
	if (LookAtActor == HitResult.GetActor())
	{
		return;
	}
	
	if(IsLocallyControlled())
	{
		if (Cast<IInteractionInterface>(LookAtActor))
		{
			IInteractionInterface::Execute_StopLookAt(LookAtActor);
		}
		if (Cast<IInteractionInterface>(HitResult.GetActor()))
		{
			IInteractionInterface::Execute_LookAt(HitResult.GetActor());
		}
	}

	
	LookAtActor = HitResult.GetActor();
}

void ATagGameCharacter::Interact()
{
	if (LookAtActor == nullptr)
	{
		return;
	}
	Server_Interact();
}

void ATagGameCharacter::DropActiveItem()
{
	if (InventoryComponent == nullptr)
	{
		return;
	}

	InventoryComponent->Server_DropItem();
}

void ATagGameCharacter::Server_Interact_Implementation()
{
	if (Cast<IInteractionInterface>(LookAtActor))
	{
		UE_LOG(LogTagCharacter, Display, TEXT("Interact called %s"),*LookAtActor->GetName());
		const FString& ObjectiveId = IInteractionInterface::Execute_Interact(LookAtActor,this);
		if (ObjectiveId.IsEmpty())
		{
			UE_LOG(LogTagCharacter,Warning,TEXT("No Objective ID!"));
			return;
		}
		Client_BroadcastObjectiveIdCalled(ObjectiveId);
	}
	
	if (ATagGameCharacter* HitCharacter = Cast<ATagGameCharacter>(LookAtActor))
	{
		if (TagComponent != nullptr && TagComponent->IsChaser())
		{
			TagComponent->Tag(HitCharacter);
		}
	}
}

void ATagGameCharacter::Client_BroadcastObjectiveIdCalled_Implementation(const FString& ObjectiveId)
{
	OnObjectiveIdCalled.Broadcast(ObjectiveId);	
}