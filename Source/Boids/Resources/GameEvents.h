#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SharedChannel.h"

#include "GameEvents.generated.h"

USTRUCT()
struct FMovementStep
{
	GENERATED_BODY()
};

UCLASS(BlueprintType)
class BOIDS_API UGameEvents : public UObject
{
	GENERATED_BODY()

public:
	TSenderChannel<FMovementStep> MovementStep = {};
};
