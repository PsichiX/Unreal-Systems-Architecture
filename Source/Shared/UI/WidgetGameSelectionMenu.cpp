#include "Shared/UI/WidgetGameSelectionMenu.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/Public/Iterator.h"

#include "Shared/UI/WidgetGameButton.h"

void UWidgetGameSelectionMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->ExitGameButton))
	{
		this->ExitGameButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnExitGame);
	}

	if (IsValid(this->GamesListView))
	{
		const auto Items =
			IterStdConst(this->GamesList)
				.Filter([](const auto& Item) { return Item.MapName.IsNone() == false; })
				.Map<UObject*>(
					[&](const auto& Item)
					{
						auto* Info =
							NewObject<UWidgetGameInfo>(this, UWidgetGameInfo::StaticClass());
						Info->GameName = Item.GameName;
						Info->MapName = Item.MapName;

						return Info;
					})
				.CollectArray();

		this->GamesListView->SetListItems(Items);
	}
}

void UWidgetGameSelectionMenu::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->ExitGameButton))
	{
		this->ExitGameButton->OnClicked.RemoveAll(this);
	}
}

void UWidgetGameSelectionMenu::OnExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Type::Quit, false);
}
