#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetScalarParameter.generated.h"

class UTextBlock;
class USlider;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSliderChanged, float, Value);

UCLASS()
class SHARED_API UWidgetScalarParameter : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnSliderChanged& OnChanged()
	{
		return ChangedEvent;
	}

	UFUNCTION()
	void SetValue(float InValue);

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void SynchronizeProperties() override;

	UFUNCTION()
	void OnSliderChanged(float InValue);

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text = nullptr;

	UPROPERTY(Meta = (BindWidget))
	USlider* Slider = nullptr;

	UPROPERTY(EditAnywhere)
	FText Title = {};

	UPROPERTY(EditAnywhere)
	float Value = 0;

	UPROPERTY(EditAnywhere)
	float MinValue = 0;

	UPROPERTY(EditAnywhere)
	float MaxValue = 1;

	FOnSliderChanged ChangedEvent;
};
