#include "Systems_Workspace/Boids/Components/VelocityComponent.h"

void UVelocityComponent::Randomize(float MinVelocity, float MaxVelocity)
{
	const auto Size = FMath::RandRange(MinVelocity, MaxVelocity);
	this->Value = FMath::VRand().GetSafeNormal() * Size;
}
