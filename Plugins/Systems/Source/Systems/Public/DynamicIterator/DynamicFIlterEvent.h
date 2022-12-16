#pragma once

#include "CoreMinimal.h"

#include "DynamicFilterEvent.generated.h"

UCLASS()
class SYSTEMS_API UDynamicFilterOutput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool Value = false;
};

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDynamicFilterEvent, UObject*, Input, UDynamicFilterOutput*, Output);
