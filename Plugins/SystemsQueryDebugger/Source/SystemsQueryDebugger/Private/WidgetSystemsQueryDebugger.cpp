#include "SystemsQueryDebugger/Public/WidgetSystemsQueryDebugger.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Editor.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsQueryDebugger/Public/WidgetSystemsQueryComponent.h"
#include "UnrealEd.h"

FString GetWorldDescription(UWorld* World);

void UWidgetSystemsQueryDebugger::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->SelectSystemsWorldComboBox))
	{
		this->SelectSystemsWorldComboBox->OnSelectionChanged.AddUniqueDynamic(
			this, &ThisClass::OnSelectedSystemsWorldChanged);
	}

	if (IsValid(this->DiscoverSystemsWorldsButton))
	{
		this->DiscoverSystemsWorldsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnDiscoverSystemsWorldsClicked);
	}

	if (IsValid(this->PerformQueryButton))
	{
		this->PerformQueryButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPerformQueryClicked);
	}

	FEditorDelegates::ResumePIE.AddUObject(this, &ThisClass::OnResumePie);
	FEditorDelegates::EndPIE.AddUObject(this, &ThisClass::OnEndPie);

	ResetState();
}

void UWidgetSystemsQueryDebugger::NativeDestruct()
{
	Super::NativeDestruct();

	ResetState();

	if (IsValid(this->SelectSystemsWorldComboBox))
	{
		this->SelectSystemsWorldComboBox->OnSelectionChanged.RemoveAll(this);
	}

	if (IsValid(this->DiscoverSystemsWorldsButton))
	{
		this->DiscoverSystemsWorldsButton->OnClicked.RemoveAll(this);
	}

	if (IsValid(this->PerformQueryButton))
	{
		this->PerformQueryButton->OnClicked.RemoveAll(this);
	}

	FEditorDelegates::ResumePIE.RemoveAll(this);
	FEditorDelegates::EndPIE.RemoveAll(this);
}

void UWidgetSystemsQueryDebugger::OnSelectedSystemsWorldChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (IsValid(this->SelectSystemsWorldComboBox) == false || IsValid(this->QueryComponentsGroup) == false)
	{
		ResetState();
		return;
	}

	const auto Found = this->SelectSystemsWorldComboBox->FindOptionIndex(SelectedItem);
	if (Found < 0 || Found >= this->SystemsWorlds.Num())
	{
		ResetState();
		return;
	}

	this->SelectedSystemsWorld = this->SystemsWorlds[Found];
	if (IsValid(this->SelectedSystemsWorld) == false)
	{
		ResetState();
		return;
	}

	const auto& Classes = this->SelectedSystemsWorld->RegisteredComponentClasses();
	this->ComponentsInfo.Reset(0);
	IterStdConst(Classes)
		.Map<UWidgetSystemsQueryComponentInfo*>(
			[](auto* Type)
			{
				auto* Info = NewObject<UWidgetSystemsQueryComponentInfo>();
				Info->Type = Type;
				Info->bEnabled = false;
				return Info;
			})
		.CollectIntoArray(this->ComponentsInfo);
	Algo::Sort(
		this->ComponentsInfo, [](auto* A, auto* B) { return A->Type->GetName().Compare(B->Type->GetName()) <= 0; });
	this->QueryComponentsListView->SetListItems(this->ComponentsInfo);
	this->QueryComponentsGroup->SetVisibility(ESlateVisibility::Visible);
}

void UWidgetSystemsQueryDebugger::OnDiscoverSystemsWorldsClicked()
{
	ResetState();

	if (IsValid(this->SelectSystemsWorldComboBox) == false)
	{
		return;
	}

	this->SystemsWorlds.Reset(0);
	for (TObjectIterator<USystemsWorld> Iter; Iter; ++Iter)
	{
		auto* Systems = *Iter;
		if (IsValid(Systems) == false)
		{
			continue;
		}

		const auto SystemsName = Systems->GetName();
		const auto WorldName = GetWorldDescription(Systems->GetWorld());

		this->SystemsWorlds.Add(Systems);
		this->SelectSystemsWorldComboBox->AddOption(FString::Printf(TEXT("%s (%s)"), *SystemsName, *WorldName));
	}

	if (this->SystemsWorlds.Num() > 0)
	{
		this->SelectSystemsWorldComboBox->SetVisibility(ESlateVisibility::Visible);
		this->SelectSystemsWorldComboBox->SetSelectedIndex(0);
	}
}

void UWidgetSystemsQueryDebugger::OnPerformQueryClicked()
{
	ResetActorsList();

	if (IsValid(this->QueryActorsGroup) == false || IsValid(this->QueryActorsListView) == false ||
		IsValid(this->SelectedSystemsWorld) == false || IsValid(this->QueryActorsCountText) == false)
	{
		return;
	}

	GUnrealEd->PlayWorld->bDebugPauseExecution = true;

	auto TaggedQuery = this->SelectedSystemsWorld->TaggedQuery();
	for (auto* Info : this->ComponentsInfo)
	{
		if (IsValid(Info) && Info->bEnabled && IsValid(Info->Type))
		{
			TaggedQuery.WithRaw(Info->Type);
		}
	}

	auto Items = TArray<AActor*>();
	TaggedQuery.Iter()
		.Map<AActor*>(
			[](const auto& QueryItem)
			{
				auto* Actor = QueryItem.Get<0>();
				return Actor;
			})
		.CollectIntoArray(Items);
	Algo::Sort(Items, [](auto* A, auto* B) { return A->GetName().Compare(B->GetName()) <= 0; });

	this->QueryActorsCountText->SetText(FText::AsNumber(Items.Num()));
	this->QueryActorsListView->SetListItems(Items);
	this->QueryActorsGroup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetSystemsQueryDebugger::OnResumePie(bool BIsSimulating)
{
	ResetActorsList();
}

void UWidgetSystemsQueryDebugger::OnEndPie(bool BIsSimulating)
{
	ResetState();
}

void UWidgetSystemsQueryDebugger::ResetState()
{
	if (IsValid(this->SelectSystemsWorldComboBox))
	{
		this->SelectSystemsWorldComboBox->ClearOptions();
		this->SelectSystemsWorldComboBox->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(this->QueryComponentsGroup))
	{
		this->QueryComponentsGroup->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(this->QueryComponentsListView))
	{
		this->QueryComponentsListView->ClearListItems();
	}

	this->SystemsWorlds.Reset(0);
	this->SelectedSystemsWorld = nullptr;
	this->ComponentsInfo.Reset(0);

	ResetActorsList();
}

void UWidgetSystemsQueryDebugger::ResetActorsList()
{
	if (IsValid(this->QueryActorsGroup))
	{
		this->QueryActorsGroup->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(this->QueryActorsListView))
	{
		this->QueryActorsListView->ClearListItems();
	}

	if (IsValid(this->QueryActorsCountText))
	{
		this->QueryActorsCountText->SetText(FText::AsNumber(0));
	}
}

FString GetWorldDescription(UWorld* World)
{
	if (IsValid(World))
	{
		const auto WorldName = World->GetFName().GetPlainNameString();

		auto InstanceIndex = TOptional<int>();
		for (const auto& Context : GEngine->GetWorldContexts())
		{
			if (Context.World() == World)
			{
				InstanceIndex = Context.PIEInstance - 1;
				break;
			}
		}

		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
				case NM_Client:
					if (InstanceIndex.IsSet())
					{
						return FString::Printf(TEXT("%s: Client %i"), *WorldName, InstanceIndex.GetValue());
					}
					else
					{
						return FString::Printf(TEXT("%s: Client"), *WorldName);
					}
				case NM_DedicatedServer:
				case NM_ListenServer:
					return FString::Printf(TEXT("%s: Server"), *WorldName);
				case NM_Standalone:
					return FString::Printf(TEXT("%s: PIE"), *WorldName);
			}
		}
		else if (World->WorldType == EWorldType::Editor)
		{
			return FString::Printf(TEXT("%s: Editor"), *WorldName);
		}
	}

	return FString(TEXT("<NO WORLD>"));
}
