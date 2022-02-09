#include "Systems_Workspace/Life/UI/WidgetGodsInterface.h"

#include "Components/ListView.h"
#include "ReactiveUserInterfaceSystems/Public/Resources/UiChangeDetection.h"
#include "Systems/Public/SystemsStatics.h"

#include "Systems_Workspace/Life/LifeGameMode.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"

void UWidgetGodsInterface::NativeConstruct()
{
	Super::NativeConstruct();

	const auto* Settings =
		USystemsStatics::GetResource<ULifeSettings>(ALifeGameMode::SYSTEMS_WORLD, GetWorld());
	if (IsValid(this->Spawnables) && IsValid(Settings) && Settings->Spawnables.Num() > 0)
	{
		auto Items = TArray<UClass*>();
		Settings->Spawnables.GetKeys(Items);
		this->Spawnables->SetListItems(Items);
	}
}
