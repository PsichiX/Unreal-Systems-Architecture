#pragma once

#include "CoreMinimal.h"

#include "DynamicComparedByEvent.generated.h"

UCLASS()
class SYSTEMS_API UDynamicComparedByOutput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool Value = false;
};

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDynamicComparedByEvent,
	UObject*,
	InputA,
	UObject*,
	InputB,
	UDynamicComparedByOutput*,
	Output);
