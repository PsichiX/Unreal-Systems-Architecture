#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "TutorialNpcComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class TUTORIAL_API UTutorialNpcComponent : public USystemsActorComponent
{
	GENERATED_BODY()
};
