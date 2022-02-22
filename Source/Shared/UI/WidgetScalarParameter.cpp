#include "Shared/UI/WidgetScalarParameter.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UWidgetScalarParameter::SetValue(float InValue)
{
	this->Value = InValue;
	if (IsValid(this->Slider))
	{
		this->Slider->SetValue(this->Value);
	}
}

void UWidgetScalarParameter::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->Slider))
	{
		this->Slider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSliderChanged);
	}
}

void UWidgetScalarParameter::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->Slider))
	{
		this->Slider->OnValueChanged.RemoveAll(this);
	}
}

void UWidgetScalarParameter::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsValid(this->Text))
	{
		this->Text->SetText(this->Title);
	}
	if (IsValid(this->Slider))
	{
		this->Slider->SetValue(this->Value);
		this->Slider->SetMinValue(this->MinValue);
		this->Slider->SetMaxValue(this->MaxValue);
	}
}

void UWidgetScalarParameter::OnSliderChanged(float InValue)
{
	this->Value = InValue;
	if (this->ChangedEvent.IsBound())
	{
		this->ChangedEvent.Broadcast(InValue);
	}
}
