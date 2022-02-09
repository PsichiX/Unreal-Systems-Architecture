#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"

#include "WidgetSystemsQueryComponent.generated.h"

class UTextBlock;
class UCheckBox;

UCLASS()
class SYSTEMSQUERYDEBUGGER_API UWidgetSystemsQueryComponentInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	const UClass* Type = nullptr;

	UPROPERTY()
	bool bEnabled = false;
};

UCLASS()
class SYSTEMSQUERYDEBUGGER_API UWidgetSystemsQueryComponent
	: public UUserWidget,
	  public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void OnEnableChanged(bool bIsChecked);

	UPROPERTY()
	UWidgetSystemsQueryComponentInfo* Info = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UCheckBox* CheckBox = nullptr;
};
