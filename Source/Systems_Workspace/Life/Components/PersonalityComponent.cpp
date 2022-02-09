#include "Systems_Workspace/Life/Components/PersonalityComponent.h"

FPersonalityTraits::FPersonalityTraits()
	: Conscientiousness(0), Agreeableness(0), Neuroticism(0), Openness(0), Extraversion(0)
{
}

FPersonalityTraits::FPersonalityTraits(float All)
	: Conscientiousness(All), Agreeableness(All), Neuroticism(All), Openness(All), Extraversion(All)
{
}

float FPersonalityTraits::Get(const EPersonalityKind Kind) const
{
	switch (Kind)
	{
		case EPersonalityKind::Conscientiousness:
			return this->Conscientiousness;
		case EPersonalityKind::Agreeableness:
			return this->Agreeableness;
		case EPersonalityKind::Neuroticism:
			return this->Neuroticism;
		case EPersonalityKind::Openness:
			return this->Openness;
		case EPersonalityKind::Extraversion:
			return this->Extraversion;
		default:
			return 0;
	}
}

void UPersonalityComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Scales.Conscientiousness =
		FMath::RandRange(this->RandomMin.Conscientiousness, this->RandomMax.Conscientiousness);

	this->Scales.Agreeableness =
		FMath::RandRange(this->RandomMin.Agreeableness, this->RandomMax.Agreeableness);

	this->Scales.Neuroticism =
		FMath::RandRange(this->RandomMin.Neuroticism, this->RandomMax.Neuroticism);

	this->Scales.Openness = FMath::RandRange(this->RandomMin.Openness, this->RandomMax.Openness);

	this->Scales.Extraversion =
		FMath::RandRange(this->RandomMin.Extraversion, this->RandomMax.Extraversion);
}
