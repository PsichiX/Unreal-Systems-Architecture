#pragma once

#include "CoreMinimal.h"

#include "TutorialPath.generated.h"

UCLASS(BlueprintType)
class TUTORIAL_API UTutorialPath : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Path = {};
};