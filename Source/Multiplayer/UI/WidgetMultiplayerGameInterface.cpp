#include "Multiplayer/UI/WidgetMultiplayerGameInterface.h"

#include "Components/TextBlock.h"

void UWidgetMultiplayerGameInterface::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(this->MultiplayerModeText))
	{
		this->MultiplayerModeText->SetText(GetWorld()->IsServer()
				? FText::AsCultureInvariant("IS SERVER")
				: FText::AsCultureInvariant("IS CLIENT"));
	}
}
