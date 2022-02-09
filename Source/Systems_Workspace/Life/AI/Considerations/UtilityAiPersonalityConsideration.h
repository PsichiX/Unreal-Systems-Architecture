#pragma once

#include "CoreMinimal.h"

#include "Systems_Workspace/Life/AI/Reasoner/UtilityAiConsideration.h"
#include "Systems_Workspace/Life/Components/PersonalityComponent.h"

#include "UtilityAiPersonalityConsideration.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SYSTEMS_WORKSPACE_API UUtilityAiPersonalityConsideration : public UUtilityAiConsideration
{
	GENERATED_BODY()

public:
	virtual float Score(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override;

	UPROPERTY(EditAnywhere)
	EPersonalityKind Kind = EPersonalityKind::Conscientiousness;

	UPROPERTY(EditAnywhere)
	float TargetValue = 0;
};
