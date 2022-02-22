#include "Life/Components/AgeComponent.h"

void UAgeComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Value = FMath::RandRange(0.0f, this->RandomMax);
	this->ApplyToSpriteMetronome.Randomize();
}
