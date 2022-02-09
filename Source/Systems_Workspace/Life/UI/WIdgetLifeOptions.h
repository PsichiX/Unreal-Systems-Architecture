#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetLifeOptions.generated.h"

class USystemsWorld;
class UWidgetCheckbox;
class UWidgetScalarParameter;

UCLASS()
class SYSTEMS_WORKSPACE_API UWidgetLifeOptions : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void OnLifeSettingsChanged(USystemsWorld& Systems);

	UFUNCTION()
	void OnHumansClearVisionChanged(bool bValue);

	UFUNCTION()
	void OnTimeScaleChanged(float Value);

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* HumansClearVision = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* TimeScale = nullptr;
};
