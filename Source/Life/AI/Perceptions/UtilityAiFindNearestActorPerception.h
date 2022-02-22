#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiPerception.h"

#include "UtilityAiFindNearestActorPerception.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiFindNearestActorPerception : public UUtilityAiPerception
{
	GENERATED_BODY()

public:
	virtual void Perceive(AActor* Actor,
		USystemsWorld& Systems,
		FUtilityAiMemory& Memory) const override;

	UPROPERTY(EditAnywhere)
	FName SourceMemoryProperty = FName(TEXT("ActorsInRange"));

	UPROPERTY(EditAnywhere)
	FName TargetMemoryProperty = FName(TEXT("NearestActor"));
};
