#include "Systems_Workspace/Boids/Components/ColorComponent.h"

const FName COLOR_PARAM_NAME = FName(TEXT("Color"));

void UColorComponent::ApplyColor(const FLinearColor Color)
{
	if (IsValid(this->CachedInstance))
	{
		this->CachedInstance->SetVectorParameterValue(COLOR_PARAM_NAME, Color);
	}
}

void UColorComponent::ApplyDefaultColor()
{
	ApplyColor(this->DefaultColor);
}

void UColorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this->MaterialInterface))
	{
		auto* Mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		if (IsValid(Mesh))
		{
			this->CachedInstance = UMaterialInstanceDynamic::Create(this->MaterialInterface, this);
			Mesh->SetMaterial(0, this->CachedInstance);
		}
	}
}
