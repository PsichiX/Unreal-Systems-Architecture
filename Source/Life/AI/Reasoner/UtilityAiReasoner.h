#pragma once

#include "CoreMinimal.h"

#include "Life/AI/Reasoner/UtilityAiConsideration.h"

#include "UtilityAiReasoner.generated.h"

class USystemsWorld;
class UUtilityAiConsideration;
class UUtilityAiTask;
class UUtilityAiBehavior;
class UUtilityAiPerception;

USTRUCT(BlueprintType)
struct LIFE_API FUtilityAiReasonerState
{
	GENERATED_BODY()

public:
	FUtilityAiReasonerState();

	FUtilityAiReasonerState(FUtilityAiMappedConsideration InConsideration,
		UUtilityAiTask* InTask,
		bool bInActiveState = true);

	UPROPERTY(EditAnywhere)
	bool bActiveState = true;

	UPROPERTY(EditAnywhere)
	FUtilityAiMappedConsideration Consideration = {};

	UPROPERTY(EditAnywhere)
	UUtilityAiTask* Task = nullptr;
};

UCLASS(BlueprintType)
class LIFE_API UUtilityAiReasoner : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static UUtilityAiReasoner* FromBehavior(UObject* Owner, UUtilityAiBehavior* Behavior);

	UFUNCTION()
	FName GetActiveState() const
	{
		return this->ActiveState;
	}

	bool ChangeActiveState(AActor* Actor,
		FName Name,
		USystemsWorld& Systems,
		FUtilityAiMemory& Memory,
		bool bForced = false);

	bool Decide(AActor* Actor, USystemsWorld& Systems);

private:
	UPROPERTY()
	TArray<UUtilityAiPerception*> Perceptions = {};

	UPROPERTY()
	TMap<FName, FUtilityAiReasonerState> States = {};

	UPROPERTY()
	FName ActiveState = FName();
};
