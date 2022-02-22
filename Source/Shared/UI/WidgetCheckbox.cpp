#include "Shared/UI/WidgetCheckbox.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

void UWidgetCheckbox::SetChecked(bool bIsChecked)
{
	this->bChecked = bIsChecked;
	if (IsValid(this->CheckBox))
	{
		this->CheckBox->SetIsChecked(this->bChecked);
	}
}

void UWidgetCheckbox::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->CheckBox))
	{
		this->CheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &ThisClass::OnCheckboxChanged);
	}
}

void UWidgetCheckbox::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->CheckBox))
	{
		this->CheckBox->OnCheckStateChanged.RemoveAll(this);
	}
}

void UWidgetCheckbox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsValid(this->Text))
	{
		this->Text->SetText(this->Title);
	}
	if (IsValid(this->CheckBox))
	{
		this->CheckBox->SetIsChecked(this->bChecked);
	}
}

void UWidgetCheckbox::OnCheckboxChanged(bool bIsChecked)
{
	this->bChecked = bIsChecked;
	if (this->ChangedEvent.IsBound())
	{
		this->ChangedEvent.Broadcast(bIsChecked);
	}
}
