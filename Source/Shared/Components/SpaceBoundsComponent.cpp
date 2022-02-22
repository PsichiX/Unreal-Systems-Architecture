#include "Shared/Components/SpaceBoundsComponent.h"

#include "Components/BoxComponent.h"

void USpaceBoundsComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto* Box = GetOwner()->FindComponentByClass<UBoxComponent>();
	if (IsValid(Box))
	{
		this->Bounds = Box->CalcBounds(GetOwner()->GetActorTransform());
	}
}
