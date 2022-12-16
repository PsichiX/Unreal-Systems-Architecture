#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "DebugSpatialQueryEpicenterComponent.generated.h"

class USystemsWorld;

UENUM(BlueprintType)
enum class EDebugSpatialQueryEpicenterMode : uint8
{
	None,
	Area,
	Radius,
};

/// Component that marks actor to be a debug spatial query epicenter.
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SYSTEMSSPATIALQUERY_API UDebugSpatialQueryEpicenterComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDebugSpatialQueryEpicenterMode Mode = EDebugSpatialQueryEpicenterMode::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Extents = FVector(100.0);
};
