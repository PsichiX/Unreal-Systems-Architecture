#pragma once

#include "CoreMinimal.h"

#include "DynamicAllEvent.generated.h"

UCLASS()
class SYSTEMS_API UDynamicAllOutput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool Value = false;
};

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDynamicAllEvent, UObject*, Input, UDynamicAllOutput*, Output);
