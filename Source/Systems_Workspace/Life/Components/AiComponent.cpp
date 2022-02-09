#include "Systems_Workspace/Life/Components/AiComponent.h"

void UAiComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this->Behavior))
	{
		this->Reasoner = UUtilityAiReasoner::FromBehavior(this, this->Behavior);
	}
}

void UAiComponent::TryDecide(USystemsWorld& Systems, float Threshold, float DeltaTime)
{
	if (IsValid(this->Reasoner))
	{
		this->DecideAccum += DeltaTime;
		if (this->Reasoner->GetActiveState().IsNone() || this->DecideAccum >= Threshold)
		{
			this->Reasoner->Decide(GetOwner(), Systems);
			this->DecideAccum = 0;
		}
	}
}
