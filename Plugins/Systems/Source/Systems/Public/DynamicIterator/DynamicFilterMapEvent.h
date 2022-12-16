#pragma once

#include "CoreMinimal.h"

#include "DynamicFilterMapEvent.generated.h"

UCLASS()
class SYSTEMS_API UDynamicFilterMapOutput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UObject* Value = nullptr;
};

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDynamicFilterMapEvent, UObject*, Input, UDynamicFilterMapOutput*, Output);
