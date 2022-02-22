#include "Shared/UI/WidgetGameButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UWidgetGameButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->Button))
	{
		this->Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClicked);
	}
}

void UWidgetGameButton::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(this->Button))
	{
		this->Button->OnClicked.RemoveAll(this);
	}
}

void UWidgetGameButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	this->GameInfo = Cast<UWidgetGameInfo>(ListItemObject);

	if (IsValid(this->GameInfo) && IsValid(this->Text))
	{
		this->Text->SetText(this->GameInfo->GameName);
	}
}

void UWidgetGameButton::OnClicked()
{
	if (IsValid(this->GameInfo))
	{
		UGameplayStatics::OpenLevel(GetWorld(), this->GameInfo->MapName);
	}
}
