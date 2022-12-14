#include "SystemsQueryDebugger/Public/WidgetSystemsQueryActor.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Editor.h"

void UWidgetSystemsQueryActor::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->SelectButton))
	{
		this->SelectButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSelectClicked);
	}
}

void UWidgetSystemsQueryActor::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->SelectButton))
	{
		this->SelectButton->OnClicked.RemoveAll(this);
	}
}

void UWidgetSystemsQueryActor::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	this->Actor = Cast<AActor>(ListItemObject);

	if (IsValid(this->Text))
	{
		if (IsValid(this->Actor))
		{
			const auto ActorName = this->Actor->GetName();
			const auto OwnerName = IsValid(this->Actor->GetOuter()) ? this->Actor->GetOuter()->GetName()
																	: FString(TEXT("<UNKNOWN OWNER>"));

			this->Text->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%s (%s)"), *ActorName, *OwnerName)));
		}
		else
		{
			this->Text->SetText(FText::AsCultureInvariant(TEXT("<INVALID ACTOR>")));
		}
	}
}

void UWidgetSystemsQueryActor::OnSelectClicked()
{
	if (IsValid(this->Actor))
	{
		GEditor->SelectActor(this->Actor, true, true, true, true);
	}
}
