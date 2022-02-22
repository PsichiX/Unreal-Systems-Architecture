#include "Life/Components/WaterComponent.h"

const FName TIME_PHASE_PARAM = FName(TEXT("TimePhase"));

void UWaterComponent::ApplyToMaterial()
{
	if (IsValid(CachedInstance))
	{
		CachedInstance->SetScalarParameterValue(TIME_PHASE_PARAM, this->Time);
	}
}

void UWaterComponent::BeginPlay()
{
	Super::BeginPlay();

	auto* Mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (IsValid(Mesh))
	{
		auto* MaterialInterface = Mesh->GetMaterial(0);
		if (IsValid(MaterialInterface))
		{
			this->CachedInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			Mesh->SetMaterial(0, this->CachedInstance);
		}
	}
}
