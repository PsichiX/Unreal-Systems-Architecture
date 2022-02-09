#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "HungerComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UHungerComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Value = 0;
};
