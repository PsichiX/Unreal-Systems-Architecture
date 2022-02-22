#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"

#include "WidgetGameButton.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SHARED_API UWidgetGameInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FText GameName = FText();

	UPROPERTY()
	FName MapName = FName();
};

UCLASS()
class SHARED_API UWidgetGameButton : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void OnClicked();

	UPROPERTY()
	UWidgetGameInfo* GameInfo = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* Button = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text = nullptr;
};
