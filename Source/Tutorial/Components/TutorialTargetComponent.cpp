#include "Tutorial/Components/TutorialTargetComponent.h"

void UTutorialTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Position = GetOwner()->GetActorLocation();
}
