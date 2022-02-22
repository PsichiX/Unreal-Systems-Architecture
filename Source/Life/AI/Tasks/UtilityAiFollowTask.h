#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiTask.h"

#include "UtilityAiFollowTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiFollowTask : public UUtilityAiTask
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bTargetCanMove = false;

	UPROPERTY(EditAnywhere)
	FName TargetMemoryProperty = FName(TEXT("NearestActor"));

private:
	virtual bool OnEnter(AActor* Actor,
		USystemsWorld& Systems,
		FUtilityAiMemory& Memory,
		bool bChanged) override;

	virtual void OnExit(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) override;
};
