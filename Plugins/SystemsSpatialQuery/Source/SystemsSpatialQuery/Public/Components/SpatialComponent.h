#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "SpatialComponent.generated.h"

class USystemsWorld;

/// Component that marks actor for spatial partitioning.
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMSSPATIALQUERY_API USpatialComponent : public USystemsActorComponent
{
	GENERATED_BODY()
};
