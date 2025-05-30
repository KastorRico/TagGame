#include "Components/TagComponent.h"

#include "TagGameCharacter.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogTagComponent,Log,All)
UTagComponent::UTagComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UTagComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTagComponent, bIsChaser);
}

void UTagComponent::Tag(ATagGameCharacter* TaggedActor)
{
	if (!bIsChaser)
	{
		return;
	}
	if (TaggedActor == GetOwner())
	{
		UE_LOG(LogTagComponent, Warning, TEXT("Character is trying to tag themselves!"));
		return;
	}
	
	ServerTagCharacter(TaggedActor);

}

void UTagComponent::ServerTagCharacter_Implementation(ATagGameCharacter* NewChaser)
{
	if (NewChaser == nullptr)
	{
		UE_LOG(LogTagComponent, Warning, TEXT("Character is not valid!"));
		return;
	}
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