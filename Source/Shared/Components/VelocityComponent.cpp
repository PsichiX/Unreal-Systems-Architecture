#include "Shared/Components/VelocityComponent.h"

#include "Net/UnrealNetwork.h"

UVelocityComponent::UVelocityComponent()
{
	SetNetAddressable();
	SetIsReplicatedByDefault(true);
}

void UVelocityComponent::Randomize(float MinVelocity, float MaxVelocity)
{
	const auto Size = FMath::RandRange(MinVelocity, MaxVelocity);
	this->Value = FMath::VRand().GetSafeNormal() * Size;
}

void UVelocityComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVelocityComponent, Value);
}
