#pragma once

#include "CoreMinimal.h"

#include "UtilityAiConsideration.generated.h"

class USystemsWorld;
struct FUtilityAiMemory;

USTRUCT(BlueprintType)
struct SYSTEMS_WORKSPACE_API FUtilityAiMappedConsideration
{
	GENERATED_BODY()

public:
	float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory);

	UPROPERTY(EditAnywhere)
	UUtilityAiConsideration* Consideration = nullptr;

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve ScoreMapping = {};
};

UCLASS(BlueprintType, Abstract)
class SYSTEMS_WORKSPACE_API UUtilityAiConsideration : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory)
	{
		return 0;
	}
};
