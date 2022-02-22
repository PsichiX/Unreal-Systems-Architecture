#include "Life/UI/WidgetSpawnerIconButton.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "PaperSprite.h"
#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsStatics.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/LifeGameMode.h"
#include "Life/Resources/LifeSettings.h"
#include "Life/Resources/LifeSpawner.h"
#include "Shared/Utils.h"

void UWidgetSpawnerIconButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->Button))
	{
		this->Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClicked);
	}

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		auto Signature = FUiChangeSignature();
		Signature.Resource<ULifeSpawner>();
		ChangeDetection->Subscribe(Signature, this, &ThisClass::OnLifeSpawnerChanged);
	}
}

void UWidgetSpawnerIconButton::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->Button))
	{
		this->Button->OnClicked.RemoveAll(this);
	}

	auto* ChangeDetection =
		USystemsStatics::GetResource<UUiChangeDetection>(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(ChangeDetection))
	{
		ChangeDetection->UnsubscribeAll(this);
	}
}

void UWidgetSpawnerIconButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	auto* Item = Cast<UClass>(ListItemObject);
	auto* Systems = USystemsStatics::GetSystemsWorld(ALifeGameMode::SYSTEMS_WORLD, GetWorld());

	if (IsValid(Item) && IsValid(Systems) && IsValid(this->Icon) && IsValid(this->Border))
	{
		const auto* Settings = Systems->Resource<ULifeSettings>();

		if (IsValid(Settings))
		{
			auto* const* SpriteOpt = Settings->Spawnables.Find(Item);

			if (SpriteOpt != nullptr && IsValid(*SpriteOpt))
			{
				auto* Sprite = *SpriteOpt;
				const auto Brush = MakeBrushFromSprite(Sprite);

				Icon->SetBrush(Brush);
				this->Spawnable = Item;
			}
		}

		// When widget gets new data it still can have "selected" state visualization on it, so we
		// have to refresh that visualization.
		OnLifeSpawnerChanged(*Systems);
	}
}

void UWidgetSpawnerIconButton::OnLifeSpawnerChanged(USystemsWorld& Systems)
{
	const auto* Spawner = Systems.Resource<ULifeSpawner>();

	if (IsValid(Spawner))
	{
		const auto bSelected = IsValid(Spawner->SelectedToSpawn) && IsValid(this->Spawnable) &&
			this->Spawnable == Spawner->SelectedToSpawn;

		this->Border->SetBrushColor(bSelected ? FLinearColor::Yellow : FLinearColor::Transparent);
	}
}

void UWidgetSpawnerIconButton::OnClicked()
{
	auto* Systems = USystemsStatics::GetSystemsWorld(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(Systems))
	{
		auto* Spawner = Systems->Resource<ULifeSpawner>();
		if (IsValid(Spawner))
		{
			Spawner->SelectedToSpawn =
				Spawner->SelectedToSpawn == this->Spawnable ? nullptr : this->Spawnable;

			Systems->MarkResourceChanged(Spawner);
		}
	}
}
