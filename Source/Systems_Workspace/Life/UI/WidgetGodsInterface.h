#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetGodsInterface.generated.h"

class UListView;

UCLASS()
class SYSTEMS_WORKSPACE_API UWidgetGodsInterface : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	UPROPERTY(Meta = (BindWidget))
	UListView* Spawnables = nullptr;
};
