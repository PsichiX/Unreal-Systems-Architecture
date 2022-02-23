#include "Life/UI/WidgetLifeOptions.h"

#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Shared/UI/WidgetCheckbox.h"
#include "Shared/UI/WidgetScalarParameter.h"
#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/LifeGameMode.h"
#include "Life/Resources/LifeSettings.h"
#include "Life/Systems/OneShot/HumansClearVisionSystem.h"

void UWidgetLifeOptions::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(HumansClearVision))
	{
		HumansClearVision->OnChanged().AddUniqueDynamic(
			this, &ThisClass::OnHumansClearVisionChanged);
	}

	if (IsValid(TimeScale))
	{
		TimeScale->OnChanged().AddUniqueDynamic(this, &ThisClass::OnTimeScaleChanged);
	}

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		auto Signature = FUiChangeSignature();
		Signature.Resource<ULifeSettings>();
		ChangeDetection->Subscribe(Signature, this, &ThisClass::OnLifeSettingsChanged);
	}
}

void UWidgetLifeOptions::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(HumansClearVision))
	{
		HumansClearVision->OnChanged().RemoveAll(this);
	}

	if (IsValid(TimeScale))
	{
		TimeScale->OnChanged().RemoveAll(this);
	}

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		ChangeDetection->UnsubscribeAll(this);
	}
}

void UWidgetLifeOptions::OnLifeSettingsChanged(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();

	if (IsValid(Settings) == false || IsValid(HumansClearVision) == false ||
		IsValid(TimeScale) == false)
	{
		return;
	}

	HumansClearVision->SetChecked(Settings->bHumansClearVision);
	TimeScale->SetValue(Settings->TimeScale);
}

void UWidgetLifeOptions::OnHumansClearVisionChanged(bool bValue)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<ULifeSettings>();
		if (IsValid(Settings))
		{
			Settings->bHumansClearVision = bValue;
			Systems->MarkResourceChanged(Settings);
			HumansClearVisionSystem(*Systems);
		}
	}
}

void UWidgetLifeOptions::OnTimeScaleChanged(float Value)
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Settings = Systems->Resource<ULifeSettings>();
		if (IsValid(Settings))
		{
			Settings->TimeScale = Value;
			Systems->MarkResourceChanged(Settings);
		}
	}
}
