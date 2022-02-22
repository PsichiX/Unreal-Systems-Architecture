#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetMultiplayerGameInterface.generated.h"

class UTextBlock;

UCLASS()
class MULTIPLAYER_API UWidgetMultiplayerGameInterface : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* MultiplayerModeText = nullptr;
};
