#pragma once

#include "CoreMinimal.h"

class USystemsWorld;

UFUNCTION()
void SpawnCursorSystem(USystemsWorld& Systems);

UFUNCTION()
void ApplyCursorToWorldSystem(USystemsWorld& Systems);
