#include "Components/TagComponent.h"

#include "TagGameCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogTagComponent,Log,All)
UTagComponent::UTagComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


void UTagComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ATagGameCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (OwnerCharacter->InputComponent != nullptr)
	{
		SetupInput();
	}
	else 
	{
		OwnerCharacter->OnInputSetup.AddUObject(this, &UTagComponent::SetupInput);
	}
}

void UTagComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTagComponent, bIsChaser);
}

void UTagComponent::Tag()
{
	if (!bIsChaser)
	{
		return;
	}

	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = Start + GetOwner()->GetActorForwardVector() * TagTraceLength;
	DrawDebugLine(GetWorld(),Start,End,FColor::Red,true);

	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->SweepSingleByObjectType(HitResult,
		Start,End,FQuat(),CollisionObjectQueryParams,
		FCollisionShape::MakeSphere(TagTraceRadius),
		CollisionQueryParams);

	if (!HitResult.bBlockingHit)
	{
		return;
	}
	ATagGameCharacter* HitCharacter = Cast<ATagGameCharacter>(HitResult.GetActor());
	if (HitCharacter == nullptr)
	{
		return;
	}
	ServerTagCharacter(HitCharacter);

}

void UTagComponent::SetupInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetOwner()->InputComponent))
	{
		EnhancedInputComponent->BindAction(TagAction, ETriggerEvent::Started, this, &UTagComponent::Tag);
	}
	else
	{
		UE_LOG(LogTagComponent, Error, TEXT("Failed to find an Enhanced Input component!"));
	}
}

void UTagComponent::ServerTagCharacter_Implementation(ATagGameCharacter* NewChaser)
{
	UTagComponent* TagComponent = NewChaser->GetTagComponent();
	if (TagComponent == nullptr)
	{
		return;
	}
	bIsChaser = false;
	TagComponent->SetIsChaser(true);
	UE_LOG(LogTagComponent, Warning, TEXT("%s is Tagged"), *NewChaser->GetName());
}

bool UTagComponent::ServerTagCharacter_Validate(ATagGameCharacter* NewChaser)
{
	return true;
}