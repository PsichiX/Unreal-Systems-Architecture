#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "ObstacleComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UObstacleComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float AvoidanceWeight = 1;
};
