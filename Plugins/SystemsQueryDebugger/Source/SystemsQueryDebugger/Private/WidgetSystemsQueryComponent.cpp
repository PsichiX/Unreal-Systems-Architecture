#include "SystemsQueryDebugger/Public/WidgetSystemsQueryComponent.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

void UWidgetSystemsQueryComponent::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->CheckBox))
	{
		this->CheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &ThisClass::OnEnableChanged);
	}
}

void UWidgetSystemsQueryComponent::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->CheckBox))
	{
		this->CheckBox->OnCheckStateChanged.RemoveAll(this);
	}
}

void UWidgetSystemsQueryComponent::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	this->Info = Cast<UWidgetSystemsQueryComponentInfo>(ListItemObject);

	if (IsValid(this->Text) == false || IsValid(this->CheckBox) == false)
	{
		return;
	}

	if (IsValid(this->Info))
	{
		this->Text->SetText(FText::AsCultureInvariant(this->Info->Type->GetName()));
		this->CheckBox->SetIsChecked(this->Info->bEnabled);
	}
	else
	{
		this->Text->SetText(FText::AsCultureInvariant(TEXT("<INVALID COMPONENT CLASS>")));
		this->CheckBox->SetIsChecked(false);
	}
}

void UWidgetSystemsQueryComponent::OnEnableChanged(bool bIsChecked)
{
	if (IsValid(this->Info))
	{
		this->Info->bEnabled = bIsChecked;
	}
}
