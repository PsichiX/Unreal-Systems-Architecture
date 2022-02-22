#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "RadiusComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class BOIDS_API URadiusComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(editAnywhere)
	float Value = 1;
};
