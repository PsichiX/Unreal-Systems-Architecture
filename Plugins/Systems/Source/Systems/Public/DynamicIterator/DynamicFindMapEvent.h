#pragma once

#include "CoreMinimal.h"

#include "DynamicFindMapEvent.generated.h"

UCLASS()
class SYSTEMS_API UDynamicFindMapOutput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UObject* Value = nullptr;
};

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDynamicFindMapEvent, UObject*, Input, UDynamicFindMapOutput*, Output);
