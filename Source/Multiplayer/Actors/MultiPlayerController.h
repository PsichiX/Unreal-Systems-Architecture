#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "MultiPlayerController.generated.h"

class USystemsWorld;

UCLASS()
class MULTIPLAYER_API AMultiPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void OnActionPressed();

	UFUNCTION()
	void OnActionReleased();

	UFUNCTION()
	void OnRelativeDirectionHorizontal(float Delta);

	UFUNCTION()
	void OnRelativeDirectionVertical(float Delta);

	UFUNCTION()
	void OnRelativeMovementHorizontal(float Delta);

	UFUNCTION()
	void OnRelativeMovementVertical(float Delta);
};
