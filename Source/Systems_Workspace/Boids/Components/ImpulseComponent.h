#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "ImpulseComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UImpulseComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FVector Value = FVector(0);
};
