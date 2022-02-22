#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "VelocityComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SHARED_API UVelocityComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UVelocityComponent();

	UFUNCTION()
	void Randomize(float MinVelocity, float MaxVelocity);

	UPROPERTY(VisibleAnywhere, Replicated)
	FVector Value = FVector(0);
};
