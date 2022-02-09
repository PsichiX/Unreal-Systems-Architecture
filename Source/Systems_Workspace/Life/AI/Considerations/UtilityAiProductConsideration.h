#pragma once

#include "CoreMinimal.h"

#include "Systems_Workspace/Life/AI/Reasoner/UtilityAiConsideration.h"

#include "UtilityAiProductConsideration.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SYSTEMS_WORKSPACE_API UUtilityAiProductConsideration : public UUtilityAiConsideration
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override;

	UPROPERTY(EditAnywhere)
	TArray<FUtilityAiMappedConsideration> Considerations = {};
};
