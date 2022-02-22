#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiConsideration.h"

#include "UtilityAiHungerConsideration.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiHungerConsideration : public UUtilityAiConsideration
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override;

	UPROPERTY(EditAnywhere)
	float Threshold = 0;
};
