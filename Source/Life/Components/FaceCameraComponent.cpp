#include "Life/Components/FaceCameraComponent.h"

void UFaceCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Metronome.Randomize();
}
