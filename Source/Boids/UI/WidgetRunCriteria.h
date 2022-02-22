#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetRunCriteria.generated.h"

class UWidgetCheckbox;
class USystemsWorld;

UCLASS()
class BOIDS_API UWidgetRunCriteria : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void OnRunCriteriaChanged(USystemsWorld& Systems);

	UFUNCTION()
	void OnRunPopulationControlChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunSeparationChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunAlignmentChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunCohesionChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunHuntChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunHungerChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunEatChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunLimitVelocityChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunKeepInSpaceBoundsChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunApplyImpulseChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunMovementChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunFaceDirectionChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunProximityColorChanged(bool bIsChecked);

	UFUNCTION()
	void OnRunDebugDrawChanged(bool bIsChecked);

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunPopulationControl = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunSeparation = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunAlignment = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunCohesion = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunHunt = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunHunger = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunEat = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunLimitVelocity = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunKeepInSpaceBounds = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunApplyImpulse = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunMovement = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunFaceDirection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunProximityColor = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UWidgetCheckbox* RunDebugDraw = nullptr;
};
