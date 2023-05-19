#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "UtilityAiPerception.generated.h"

class USystemsWorld;
struct FUtilityAiMemory;

UCLASS(BlueprintType, Abstract)
class LIFE_API UUtilityAiPerception : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void Perceive(AActor* Actor, USystemsWorld& Systems, FUtilityAiMemory& Memory) const
	{
	}
};