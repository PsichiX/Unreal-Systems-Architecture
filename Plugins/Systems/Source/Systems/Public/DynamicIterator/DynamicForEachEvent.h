#pragma once

#include "CoreMinimal.h"

#include "DynamicForEachEvent.generated.h"

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_OneParam(FDynamicForEachEvent, UObject*, Input);
