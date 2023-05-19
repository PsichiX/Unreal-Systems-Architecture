#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "UtilityAiTask.generated.h"

class USystemsWorld;
struct FUtilityAiMemory;

UCLASS(BlueprintType, Abstract)
class LIFE_API UUtilityAiTask : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual bool IsLocked(AActor* Actor, USystemsWorld& Systems) const
	{
		return false;
	}

	virtual bool OnEnter(AActor* Actor,
		USystemsWorld& Systems,
		FUtilityAiMemory& Memory,
		bool bChanged);

	virtual void OnExit(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory);

private:
	UPROPERTY(EditDefaultsOnly)
	TSet<TSubclassOf<UActorComponent>> RegisterComponents = {};
};

UCLASS(BlueprintType, Blueprintable)
class LIFE_API UUtilityAiIdleTask : public UUtilityAiTask
{
	GENERATED_BODY()
};