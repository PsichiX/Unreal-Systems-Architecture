#pragma once

#include "CoreMinimal.h"

#include "MultiPlayerInput.generated.h"

UCLASS(BlueprintType)
class MULTIPLAYER_API UMultiPlayerInput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	bool bAction = false;

	UPROPERTY(VisibleAnywhere)
	FVector2D RelativeMovement = FVector2D(0);

	UPROPERTY(VisibleAnywhere)
	FVector2D RelativeLook = FVector2D(0);
};
