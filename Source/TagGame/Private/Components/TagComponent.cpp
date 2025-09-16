#include "Components/TagComponent.h"

#include "Player/TagGameCharacter.h"
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

void UTagComponent::Tag(const ATagGameCharacter* TaggedActor)
{
	if (!bIsChaser)
	{
		UE_LOG(LogTagComponent, Warning, TEXT("Character is not a chaser!"));
		return;
	}
	if (TaggedActor == GetOwner())
	{
		UE_LOG(LogTagComponent, Warning, TEXT("Character is trying to tag themselves!"));
		return;
	}
	if (TaggedActor == nullptr)
	{
		UE_LOG(LogTagComponent, Warning, TEXT("Character is not valid!"));
		return;
	}
	
	UTagComponent* TagComponent = TaggedActor->GetTagComponent();
	if (TagComponent == nullptr)
	{
		return;
	}
	bIsChaser = false;
	TagComponent->SetIsChaser(true);
	UE_LOG(LogTagComponent, Warning, TEXT("%s is Tagged"), *TaggedActor->GetName());

}
