#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiConsideration.h"

#include "UtilityAiDistanceToConsideration.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiDistanceToConsideration : public UUtilityAiConsideration
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override;

	UPROPERTY(EditAnywhere)
	FName ActorMemoryProperty = FName();
};
