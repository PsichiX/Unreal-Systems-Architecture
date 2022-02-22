#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetLifeStats.generated.h"

class USystemsWorld;
class UTextBlock;

UCLASS()
class LIFE_API UWidgetLifeStats : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void OnHumansChanged(USystemsWorld& Systems);

	void OnAnimalsChanged(USystemsWorld& Systems);

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* HumansCount = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* AnimalsCount = nullptr;
};
