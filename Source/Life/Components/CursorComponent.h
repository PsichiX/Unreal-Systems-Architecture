#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "CursorComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class LIFE_API UCursorComponent : public USystemsActorComponent
{
	GENERATED_BODY()
};
