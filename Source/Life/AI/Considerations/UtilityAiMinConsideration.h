#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiConsideration.h"

#include "UtilityAiMinConsideration.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiMinConsideration : public UUtilityAiConsideration
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override;

	UPROPERTY(EditAnywhere)
	TArray<FUtilityAiMappedConsideration> Considerations = {};
};
