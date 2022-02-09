#include "Systems_Workspace/Life/Components/FaceCameraComponent.h"

void UFaceCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Metronome.Randomize();
}
