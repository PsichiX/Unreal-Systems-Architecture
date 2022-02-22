#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiTask.h"

#include "UtilityAiWanderTask.generated.h"

struct FutilityAiMemory;

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiWanderTask : public UUtilityAiTask
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Range = 0;

private:
	virtual bool OnEnter(AActor* Actor,
		USystemsWorld& Systems,
		FUtilityAiMemory& Memory,
		bool bChanged) override;

	virtual void OnExit(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override;
};
