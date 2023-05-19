#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "UtilityAiConsideration.generated.h"

class USystemsWorld;
struct FUtilityAiMemory;

USTRUCT(BlueprintType)
struct LIFE_API FUtilityAiMappedConsideration
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
class LIFE_API UUtilityAiConsideration : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory)
	{
		return 0;
	}
};