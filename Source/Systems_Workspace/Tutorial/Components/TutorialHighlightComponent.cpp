#include "Systems_Workspace/Tutorial/Components/TutorialHighlightComponent.h"

const FName COLOR_PARAM_NAME = FName(TEXT("Color"));

void UTutorialHighlightComponent::ApplyHighlighted(bool bMode)
{
	if (IsValid(this->CachedInstance))
	{
		const auto Color = bMode ? this->HighlightColor : this->DefaultColor;
		this->CachedInstance->SetVectorParameterValue(COLOR_PARAM_NAME, Color);
	}
}

void UTutorialHighlightComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this->MaterialInterface))
	{
		auto* Mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		if (IsValid(Mesh))
		{
			this->CachedInstance = UMaterialInstanceDynamic::Create(this->MaterialInterface, this);
			Mesh->SetMaterial(0, this->CachedInstance);

			ApplyHighlighted(false);
		}
	}
}
