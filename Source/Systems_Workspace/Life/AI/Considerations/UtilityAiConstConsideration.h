#pragma once

#include "CoreMinimal.h"

#include "Systems_Workspace/Life/AI/Reasoner/UtilityAiConsideration.h"

#include "UtilityAiConstConsideration.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SYSTEMS_WORKSPACE_API UUtilityAiConstConsideration : public UUtilityAiConsideration
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
