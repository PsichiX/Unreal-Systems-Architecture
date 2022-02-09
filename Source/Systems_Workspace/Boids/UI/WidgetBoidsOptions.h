#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetBoidsOptions.generated.h"

class UWidgetScalarParameter;
class UWidgetVariantParameter;
class USystemsWorld;

UCLASS()
class SYSTEMS_WORKSPACE_API UWidgetBoidsOptions : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void OnBoidsSettingsChanged(USystemsWorld& Systems);

	UFUNCTION()
	void OnTimeScaleChanged(float InValue);

	UFUNCTION()
	void OnSeparationPerceptionRangeChanged(float InValue);

	UFUNCTION()
	void OnAlignmentPerceptionRangeChanged(float InValue);

	UFUNCTION()
	void OnCohesionPerceptionRangeChanged(float InValue);

	UFUNCTION()
	void OnHuntPerceptionRangeChanged(float InValue);

	UFUNCTION()
	void OnVisibilityConeAngleDegreesChanged(float InValue);

	UFUNCTION()
	void OnAlignmentFactorChanged(float InValue);

	UFUNCTION()
	void OnCohesionFactorChanged(float InValue);

	UFUNCTION()
	void OnHuntFactorChanged(float InValue);

	UFUNCTION()
	void OnSeparationFactorChanged(float InValue);

	UFUNCTION()
	void OnMinVelocityChanged(float InValue);

	UFUNCTION()
	void OnMaxVelocityChanged(float InValue);

	UFUNCTION()
	void OnPopulationNumberChanged(float InValue);

	UFUNCTION()
	void OnProximityColorModeChanged(FName InValue);

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* TimeScale = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* SeparationPerceptionRange = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* AlignmentPerceptionRange = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* CohesionPerceptionRange = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* HuntPerceptionRange = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* VisibilityConeAngleDegrees = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* AlignmentFactor = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* CohesionFactor = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* HuntFactor = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* SeparationFactor = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* MinVelocity = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* MaxVelocity = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetScalarParameter* PopulationNumber = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetVariantParameter* ProximityColorMode = nullptr;
};
