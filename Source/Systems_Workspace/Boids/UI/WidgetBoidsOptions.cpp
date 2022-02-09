#include "Systems_Workspace/Boids/UI/WidgetBoidsOptions.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/BoidsGameMode.h"
#include "Systems_Workspace/Boids/Resources/BoidsSettings.h"
#include "Systems_Workspace/Shared/UI/WidgetScalarParameter.h"
#include "Systems_Workspace/Shared/UI/WidgetVariantParameter.h"

void UWidgetBoidsOptions::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->TimeScale))
	{
		this->TimeScale->OnChanged().AddUniqueDynamic(this, &ThisClass::OnTimeScaleChanged);
	}
	if (IsValid(this->SeparationPerceptionRange))
	{
		this->SeparationPerceptionRange->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnSeparationPerceptionRangeChanged);
	}
	if (IsValid(this->AlignmentPerceptionRange))
	{
		this->AlignmentPerceptionRange->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnAlignmentPerceptionRangeChanged);
	}
	if (IsValid(this->CohesionPerceptionRange))
	{
		this->CohesionPerceptionRange->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnCohesionPerceptionRangeChanged);
	}
	if (IsValid(this->HuntPerceptionRange))
	{
		this->HuntPerceptionRange->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnHuntPerceptionRangeChanged);
	}
	if (IsValid(this->VisibilityConeAngleDegrees))
	{
		this->VisibilityConeAngleDegrees->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnVisibilityConeAngleDegreesChanged);
	}
	if (IsValid(this->AlignmentFactor))
	{
		this->AlignmentFactor->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnAlignmentFactorChanged);
	}
	if (IsValid(this->CohesionFactor))
	{
		this->CohesionFactor->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnCohesionFactorChanged);
	}
	if (IsValid(this->HuntFactor))
	{
		this->HuntFactor->OnChanged().AddUniqueDynamic(this, &ThisClass::OnHuntFactorChanged);
	}
	if (IsValid(this->SeparationFactor))
	{
		this->SeparationFactor->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnSeparationFactorChanged);
	}
	if (IsValid(this->MinVelocity))
	{
		this->MinVelocity->OnChanged().AddUniqueDynamic(this, &ThisClass::OnMinVelocityChanged);
	}
	if (IsValid(this->MaxVelocity))
	{
		this->MaxVelocity->OnChanged().AddUniqueDynamic(this, &ThisClass::OnMaxVelocityChanged);
	}
	if (IsValid(this->PopulationNumber))
	{
		this->PopulationNumber->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnPopulationNumberChanged);
	}
	if (IsValid(this->ProximityColorMode))
	{
		this->ProximityColorMode->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnProximityColorModeChanged);
	}

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		auto Signature = FUiChangeSignature();
		Signature.Resource<UBoidsSettings>();
		ChangeDetection->Subscribe(Signature, this, &ThisClass::OnBoidsSettingsChanged);
	}
}

void UWidgetBoidsOptions::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->TimeScale))
	{
		this->TimeScale->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->SeparationPerceptionRange))
	{
		this->SeparationPerceptionRange->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->AlignmentPerceptionRange))
	{
		this->AlignmentPerceptionRange->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->CohesionPerceptionRange))
	{
		this->CohesionPerceptionRange->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->HuntPerceptionRange))
	{
		this->HuntPerceptionRange->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->VisibilityConeAngleDegrees))
	{
		this->VisibilityConeAngleDegrees->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->AlignmentFactor))
	{
		this->AlignmentFactor->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->CohesionFactor))
	{
		this->CohesionFactor->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->HuntFactor))
	{
		this->HuntFactor->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->SeparationFactor))
	{
		this->SeparationFactor->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->MinVelocity))
	{
		this->MinVelocity->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->MaxVelocity))
	{
		this->MaxVelocity->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->PopulationNumber))
	{
		this->PopulationNumber->OnChanged().RemoveAll(this);
	}
	if (IsValid(this->ProximityColorMode))
	{
		this->ProximityColorMode->OnChanged().RemoveAll(this);
	}

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		ChangeDetection->UnsubscribeAll(this);
	}
}

void UWidgetBoidsOptions::OnBoidsSettingsChanged(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<UBoidsSettings>();

	if (IsValid(Settings) == false)
	{
		return;
	}

	if (IsValid(this->TimeScale))
	{
		this->TimeScale->SetValue(Settings->TimeScale);
	}
	if (IsValid(this->SeparationPerceptionRange))
	{
		this->SeparationPerceptionRange->SetValue(Settings->SeparationPerceptionRange);
	}
	if (IsValid(this->AlignmentPerceptionRange))
	{
		this->AlignmentPerceptionRange->SetValue(Settings->AlignmentPerceptionRange);
	}
	if (IsValid(this->CohesionPerceptionRange))
	{
		this->CohesionPerceptionRange->SetValue(Settings->CohesionPerceptionRange);
	}
	if (IsValid(this->HuntPerceptionRange))
	{
		this->HuntPerceptionRange->SetValue(Settings->HuntPerceptionRange);
	}
	if (IsValid(this->VisibilityConeAngleDegrees))
	{
		this->VisibilityConeAngleDegrees->SetValue(Settings->VisibilityConeAngleDegrees);
	}
	if (IsValid(this->AlignmentFactor))
	{
		this->AlignmentFactor->SetValue(Settings->AlignmentFactor);
	}
	if (IsValid(this->CohesionFactor))
	{
		this->CohesionFactor->SetValue(Settings->CohesionFactor);
	}
	if (IsValid(this->HuntFactor))
	{
		this->HuntFactor->SetValue(Settings->HuntFactor);
	}
	if (IsValid(this->SeparationFactor))
	{
		this->SeparationFactor->SetValue(Settings->SeparationFactor);
	}
	if (IsValid(this->MinVelocity))
	{
		this->MinVelocity->SetValue(Settings->MinVelocity);
	}
	if (IsValid(this->MaxVelocity))
	{
		this->MaxVelocity->SetValue(Settings->MaxVelocity);
	}
	if (IsValid(this->PopulationNumber))
	{
		this->PopulationNumber->SetValue(static_cast<float>(Settings->PopulationNumber));
	}
	if (IsValid(this->ProximityColorMode))
	{
		this->ProximityColorMode->SetValue(static_cast<int>(Settings->ProximityColorMode));
	}
}

void UWidgetBoidsOptions::OnTimeScaleChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->TimeScale = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnSeparationPerceptionRangeChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->SeparationPerceptionRange = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnAlignmentPerceptionRangeChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->AlignmentPerceptionRange = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnCohesionPerceptionRangeChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->CohesionPerceptionRange = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnHuntPerceptionRangeChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->HuntPerceptionRange = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnVisibilityConeAngleDegreesChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->VisibilityConeAngleDegrees = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnAlignmentFactorChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->AlignmentFactor = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnCohesionFactorChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->CohesionFactor = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnHuntFactorChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->HuntFactor = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnSeparationFactorChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->SeparationFactor = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnMinVelocityChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->MinVelocity = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnMaxVelocityChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->MaxVelocity = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnPopulationNumberChanged(float InValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<UBoidsSettings>();
		if (IsValid(Settings))
		{
			Settings->PopulationNumber = InValue;
			Systems->MarkResourceChanged(Settings);
		}
	}
}

void UWidgetBoidsOptions::OnProximityColorModeChanged(FName InValue)
{
	const auto* Enum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBoidsProximityColorMode"), true);
	if (IsValid(Enum))
	{
		const auto Value = Enum->GetValueByName(InValue);
		if (Value != INDEX_NONE)
		{
			auto* Systems =
				USystemsStatics::GetSystemsWorld(ABoidsGameMode::SYSTEMS_WORLD, GetWorld());
			if (IsValid(Systems))
			{
				auto* Settings = Systems->Resource<UBoidsSettings>();
				if (IsValid(Settings))
				{
					Settings->ProximityColorMode = static_cast<EBoidsProximityColorMode>(Value);
					Systems->MarkResourceChanged(Settings);
				}
			}
		}
	}
}
