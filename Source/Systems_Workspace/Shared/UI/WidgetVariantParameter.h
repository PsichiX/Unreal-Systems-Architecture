#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetVariantParameter.generated.h"

class UTextBlock;
class UComboBoxString;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVariantChanged, FName, Name);

UCLASS()
class SYSTEMS_WORKSPACE_API UWidgetVariantParameter : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnVariantChanged& OnChanged()
	{
		return ChangedEvent;
	}

	UFUNCTION()
	void SetValue(int Index);

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void SynchronizeProperties() override;

	UFUNCTION()
	void OnVariantChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UComboBoxString* ComboBox = nullptr;

	UPROPERTY(EditAnywhere)
	FText Title = {};

	UPROPERTY(EditAnywhere)
	TArray<FName> Variants = {};

	UPROPERTY(EditAnywhere)
	FName Value = {};

	FOnVariantChanged ChangedEvent;
};
