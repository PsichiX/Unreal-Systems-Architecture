#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "ClearRangeComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UClearRangeComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float RangeMin = 0;

	UPROPERTY(EditAnywhere)
	float RangeMax = 1000;

	UPROPERTY(EditAnywhere)
	bool bScreenSpace = false;
};
