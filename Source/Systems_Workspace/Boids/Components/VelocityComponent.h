#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "VelocityComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UVelocityComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Randomize(float MinVelocity, float MaxVelocity);

	UPROPERTY()
	FVector Value = FVector(0);
};
