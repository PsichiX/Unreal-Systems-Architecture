#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"

#include "WidgetSystemsQueryActor.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SYSTEMSQUERYDEBUGGER_API UWidgetSystemsQueryActor : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void OnSelectClicked();

	UPROPERTY()
	AActor* Actor = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* SelectButton = nullptr;
};
