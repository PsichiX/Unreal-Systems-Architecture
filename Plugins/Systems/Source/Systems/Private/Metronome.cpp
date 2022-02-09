#include "Systems/Public/Metronome.h"

void FMetronome::Randomize()
{
	this->Accumulator = FMath::RandRange(0.0f, this->Limit);
}

void FMetronome::Progress(float DeltaTime)
{
	this->Accumulator += DeltaTime;
	if (this->Accumulator >= this->Limit)
	{
		this->Accumulator -= this->Limit;
		this->bTicked = true;
	}
}

bool FMetronome::ConsumeTicked()
{
	const auto Result = this->bTicked;
	this->bTicked = false;
	return Result;
}
