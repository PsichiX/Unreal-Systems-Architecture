#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "Life/AI/Reasoner/UtilityAiConsideration.h"

#include "UtilityAiBehavior.generated.h"

struct FUtilityAiReasonerState;
class UUtilityAiConsideration;
class UUtilityAiTask;
class UUtilityAiPerception;

USTRUCT(BlueprintType)
struct LIFE_API FUtilityAiBehaviorState
{
	GENERATED_BODY()

public:
	TOptional<FUtilityAiReasonerState> Instantiate(UObject* Owner) const;

	UPROPERTY(EditAnywhere)
	bool bActiveState = true;

	UPROPERTY(EditAnywhere)
	FUtilityAiMappedConsideration Consideration = {};

	UPROPERTY(EditAnywhere)
	UUtilityAiTask* Task = nullptr;
};

UCLASS(BlueprintType)
class LIFE_API UUtilityAiBehavior : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<UUtilityAiPerception*> Perceptions = {};

	UPROPERTY(EditAnywhere)
	TMap<FName, FUtilityAiBehaviorState> States = {};
};