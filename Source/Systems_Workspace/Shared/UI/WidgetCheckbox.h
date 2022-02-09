#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetCheckbox.generated.h"

class UTextBlock;
class UCheckBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckboxChanged, bool, bIsChecked);

UCLASS()
class SYSTEMS_WORKSPACE_API UWidgetCheckbox : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnCheckboxChanged& OnChanged()
	{
		return ChangedEvent;
	}

	void SetChecked(bool bisChecked);

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void SynchronizeProperties() override;

	UFUNCTION()
	void OnCheckboxChanged(bool bIsChecked);

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UCheckBox* CheckBox = nullptr;

	UPROPERTY(EditAnywhere)
	FText Title = {};

	UPROPERTY(EditAnywhere)
	bool bChecked = false;

	FOnCheckboxChanged ChangedEvent;
};
