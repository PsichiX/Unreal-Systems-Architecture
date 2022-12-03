#include "SystemsQueryDebugger/Public/SystemsQueryDebugger.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Editor.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "UObject/ConstructorHelpers.h"
#include "WidgetSystemsQueryDebugger.h"

#define LOCTEXT_NAMESPACE "FSystemsQueryDebuggerModule"

void FSystemsQueryDebuggerModule::StartupModule()
{
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this, &FSystemsQueryDebuggerModule::RegisterMenus));
}

void FSystemsQueryDebuggerModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);
}

void FSystemsQueryDebuggerModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	auto* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	auto& Section = Menu->AddSection(
		"Systems", FText::FromString(TEXT("Systems Architecture")));
	auto Action = FExecuteAction();
	Action.BindLambda(
		[]()
		{
			auto AssetData = IAssetRegistry::GetChecked().GetAssetByObjectPath(
				FSoftObjectPath(FString(TEXT(
					"/SystemsQueryDebugger/"
					"EWBP_SystemsQueryDebugger.EWBP_SystemsQueryDebugger"))));
			auto* EditorWidget =
				Cast<UEditorUtilityWidgetBlueprint>(AssetData.GetAsset());

			if (IsValid(EditorWidget))
			{
				GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()
					->SpawnAndRegisterTab(EditorWidget);
			}
		});

	Section.AddMenuEntry("OpenSystemsQueryDebugger",
		FText::FromString(TEXT("Systems Query Debugger")),
		FText::FromString(TEXT("Opens Systems Query Debugger")),
		FSlateIcon(),
		FToolUIActionChoice(Action));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSystemsQueryDebuggerModule, SystemsQueryDebugger)