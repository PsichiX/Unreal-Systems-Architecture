#pragma once

#include "CoreMinimal.h"

#include "EditorUtilityWidget.h"

#include "WidgetSystemsQueryDebugger.generated.h"

class UListView;
class UBorder;
class UButton;
class UComboBoxString;
class UTextBlock;
class USystemsWorld;
class UWidgetSystemsQueryComponentInfo;

UCLASS()
class SYSTEMSQUERYDEBUGGER_API UWidgetSystemsQueryDebugger
	: public UEditorUtilityWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnSelectedSystemsWorldChanged(FString SelectedItem,
		ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnDiscoverSystemsWorldsClicked();

	UFUNCTION()
	void OnPerformQueryClicked();

	UFUNCTION()
	void OnResumePie(bool bIsSimulating);

	UFUNCTION()
	void OnEndPie(bool bIsSimulating);

	void ResetState();

	void ResetActorsList();

	UPROPERTY()
	TArray<USystemsWorld*> SystemsWorlds = {};

	UPROPERTY()
	USystemsWorld* SelectedSystemsWorld = nullptr;

	UPROPERTY()
	TArray<UWidgetSystemsQueryComponentInfo*> ComponentsInfo = {};

	UPROPERTY(Meta = (BindWidget))
	UComboBoxString* SelectSystemsWorldComboBox = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* DiscoverSystemsWorldsButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UBorder* QueryComponentsGroup = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UListView* QueryComponentsListView = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* PerformQueryButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UBorder* QueryActorsGroup = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* QueryActorsCountText = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UListView* QueryActorsListView = nullptr;
};
