#pragma once

#include "CoreMinimal.h"

#include "DynamicMapEvent.generated.h"

UCLASS()
class SYSTEMS_API UDynamicMapOutput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UObject* Value = nullptr;
};

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDynamicMapEvent, UObject*, Input, UDynamicMapOutput*, Output);
