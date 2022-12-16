#pragma once

#include "CoreMinimal.h"

#include "DynamicAnyEvent.generated.h"

UCLASS()
class SYSTEMS_API UDynamicAnyOutput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool Value = false;
};

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDynamicAnyEvent, UObject*, Input, UDynamicAnyOutput*, Output);
