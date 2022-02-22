#include "Life/UI/WidgetLifeStats.h"

#include "Components/TextBlock.h"
#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/AnimalComponent.h"
#include "Life/Components/HumanComponent.h"
#include "Life/LifeGameMode.h"

void UWidgetLifeStats::NativeConstruct()
{
	Super::NativeConstruct();

	auto* Systems = USystemsStatics::GetSystemsWorld(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* ChangeDetection = Systems->Resource<UUiChangeDetection>();
		if (IsValid(ChangeDetection))
		{
			auto Signature = FUiChangeSignature();
			Signature.Component<UHumanComponent>(Systems);
			ChangeDetection->Subscribe(Signature, this, &ThisClass::OnHumansChanged);

			Signature = FUiChangeSignature();
			Signature.Component<UAnimalComponent>(Systems);
			ChangeDetection->Subscribe(Signature, this, &ThisClass::OnAnimalsChanged);
		}
	}
}

void UWidgetLifeStats::NativeDestruct()
{
	Super::NativeDestruct();

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		ChangeDetection->UnsubscribeAll(this);
	}
}

void UWidgetLifeStats::OnHumansChanged(USystemsWorld& Systems)
{
	if (IsValid(HumansCount))
	{
		const auto Count = Systems.Query<UHumanComponent>().Count();
		HumansCount->SetText(FText::AsNumber(Count));
	}
}

void UWidgetLifeStats::OnAnimalsChanged(USystemsWorld& Systems)
{
	if (IsValid(AnimalsCount))
	{
		const auto Count = Systems.Query<UAnimalComponent>().Count();
		AnimalsCount->SetText(FText::AsNumber(Count));
	}
}
