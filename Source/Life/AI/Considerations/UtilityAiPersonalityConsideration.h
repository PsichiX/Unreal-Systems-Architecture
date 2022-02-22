#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiConsideration.h"
#include "Life/Components/PersonalityComponent.h"

#include "UtilityAiPersonalityConsideration.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiPersonalityConsideration : public UUtilityAiConsideration
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override;

	UPROPERTY(EditAnywhere)
	EPersonalityKind Kind = EPersonalityKind::Conscientiousness;

	UPROPERTY(EditAnywhere)
	float TargetValue = 0;
};
