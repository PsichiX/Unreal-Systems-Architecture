#include "Life/Components/FlipbookProxyComponent.h"

#include "Systems/Public/SystemsStatics.h"

void UFlipbookProxyComponent::ApplyVisibility(float Factor)
{
	auto Color = this->GetSpriteColor();
	Color.A = Factor;
	this->SetSpriteColor(Color);
}

void UFlipbookProxyComponent::ApplyShiny(float Factor)
{
	auto Color = this->GetSpriteColor();
	Color.R = Factor;
	this->SetSpriteColor(Color);
}

void UFlipbookProxyComponent::ApplySaturation(float Factor)
{
	auto Color = this->GetSpriteColor();
	Color.G = Factor;
	this->SetSpriteColor(Color);
}

void UFlipbookProxyComponent::BeginPlay()
{
	Super::BeginPlay();

	USystemsStatics::AddComponentEverywhere(this, GetWorld());
}

void UFlipbookProxyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	USystemsStatics::RemoveComponent(this, GetWorld());
}
