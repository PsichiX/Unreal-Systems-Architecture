#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "WidgetBoidsInterface.generated.h"

class UButton;

UCLASS()
class BOIDS_API UWidgetBoidsInterface : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnMovementStepAction();

	UFUNCTION()
	void OnSpawnFoodAction();

	UPROPERTY(Meta = (BindWidget))
	UButton* MovementStep = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* SpawnFood = nullptr;
};
