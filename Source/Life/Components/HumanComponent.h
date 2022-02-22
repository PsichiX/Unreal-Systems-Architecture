#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "HumanComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class LIFE_API UHumanComponent : public USystemsActorComponent
{
	GENERATED_BODY()
};
