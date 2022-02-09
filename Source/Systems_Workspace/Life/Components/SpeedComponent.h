#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "SpeedComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API USpeedComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Value = 100;
};
