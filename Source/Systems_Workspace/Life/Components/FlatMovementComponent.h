#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "FlatMovementComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UFlatMovementComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FVector2D Value = FVector2D(0);
};
