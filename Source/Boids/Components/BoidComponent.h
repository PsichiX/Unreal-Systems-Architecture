#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "BoidComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class BOIDS_API UBoidComponent : public USystemsActorComponent
{
	GENERATED_BODY()
};
