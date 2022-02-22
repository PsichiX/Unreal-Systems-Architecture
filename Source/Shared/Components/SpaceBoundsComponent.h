#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "SpaceBoundsComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SHARED_API USpaceBoundsComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	TOptional<FBoxSphereBounds> Bounds = {};

private:
	virtual void BeginPlay() override;
};
