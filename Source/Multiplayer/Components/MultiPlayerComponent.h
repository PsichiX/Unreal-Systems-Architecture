#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "MultiPlayerComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class MULTIPLAYER_API UMultiPlayerComponent : public USystemsActorComponent
{
	GENERATED_BODY()
};
