#pragma once

#include "CoreMinimal.h"

#include "DynamicFindEvent.generated.h"

UCLASS()
class SYSTEMS_API UDynamicFindOutput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool Value = false;
};

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDynamicFindEvent, UObject*, Input, UDynamicFindOutput*, Output);
