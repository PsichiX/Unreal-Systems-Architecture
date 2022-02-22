#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiConsideration.h"

#include "UtilityAiConstConsideration.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiConstConsideration : public UUtilityAiConsideration
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override
	{
		return this->Value;
	}

	UPROPERTY(EditAnywhere)
	float Value = 0;
};
