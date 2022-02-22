#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiPerception.h"

#include "UtilityAiActorsInRangePerception.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiActorsInRangePerception : public UUtilityAiPerception
{
	GENERATED_BODY()

public:
	virtual void Perceive(AActor* Actor,
		USystemsWorld& Systems,
		FUtilityAiMemory& Memory) const override;

	UPROPERTY(EditAnywhere)
	FName TargetMemoryProperty = FName(TEXT("ActorsInRange"));

	UPROPERTY(EditAnywhere)
	float RangeMin = 0;

	UPROPERTY(EditAnywhere)
	float RangeMax = 10000;

	UPROPERTY(EditAnywhere)
	TSet<TSubclassOf<UActorComponent>> RequiredComponents = {};
};
