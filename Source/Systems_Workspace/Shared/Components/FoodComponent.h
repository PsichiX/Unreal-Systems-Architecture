#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "FoodComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMS_WORKSPACE_API UFoodComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Amount = 1;
};
