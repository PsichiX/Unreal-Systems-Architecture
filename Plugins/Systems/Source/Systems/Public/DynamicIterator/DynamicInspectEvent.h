#pragma once

#include "CoreMinimal.h"

#include "DynamicInspectEvent.generated.h"

UDELEGATE(BlueprintType)
DECLARE_DYNAMIC_DELEGATE_OneParam(FDynamicInspectEvent, UObject*, Input);
