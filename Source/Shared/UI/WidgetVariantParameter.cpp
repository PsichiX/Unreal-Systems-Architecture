#include "Shared/UI/WidgetVariantParameter.h"

#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"

void UWidgetVariantParameter::SetValue(int Index)
{
	if (Index >= 0 && Index < this->Variants.Num())
	{
		this->Value = this->Variants[Index];
		if (IsValid(this->ComboBox))
		{
			this->ComboBox->SetSelectedOption(this->Value.ToString());
		}
	}
}

void UWidgetVariantParameter::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->ComboBox))
	{
		this->ComboBox->OnSelectionChanged.AddUniqueDynamic(this, &ThisClass::OnVariantChanged);
	}
}

void UWidgetVariantParameter::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->ComboBox))
	{
		this->ComboBox->OnSelectionChanged.RemoveAll(this);
	}
}

void UWidgetVariantParameter::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsValid(this->Text))
	{
		this->Text->SetText(this->Title);
	}
	if (IsValid(this->ComboBox))
	{
		this->ComboBox->SetSelectedOption(this->Value.ToString());
		this->ComboBox->ClearOptions();
		for (const auto Variant : this->Variants)
		{
			this->ComboBox->AddOption(Variant.ToString());
		}
	}
}

void UWidgetVariantParameter::OnVariantChanged(FString SelectedItem,
	ESelectInfo::Type SelectionType)
{
	const auto Name = FName(SelectedItem);
	if (this->Variants.Contains(Name))
	{
		this->Value = Name;
		if (this->ChangedEvent.IsBound())
		{
			this->ChangedEvent.Broadcast(Name);
		}
	}
}
