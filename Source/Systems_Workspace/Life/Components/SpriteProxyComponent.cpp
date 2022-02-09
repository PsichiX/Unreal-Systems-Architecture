#include "Systems_Workspace/Life/Components/SpriteProxyComponent.h"

#include "Systems/Public/SystemsStatics.h"

void USpriteProxyComponent::ApplyVisibility(float Factor)
{
	auto Color = this->GetSpriteColor();
	Color.A = Factor;
	this->SetSpriteColor(Color);
}

void USpriteProxyComponent::ApplyShiny(float Factor)
{
	auto Color = this->GetSpriteColor();
	Color.R = Factor;
	this->SetSpriteColor(Color);
}

void USpriteProxyComponent::ApplySaturation(float Factor)
{
	auto Color = this->GetSpriteColor();
	Color.G = Factor;
	this->SetSpriteColor(Color);
}

void USpriteProxyComponent::BeginPlay()
{
	Super::BeginPlay();

	USystemsStatics::AddComponentEverywhere(this, GetWorld());
}

void USpriteProxyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	USystemsStatics::RemoveComponent(this, GetWorld());
}
