#pragma once

#include "CoreMinimal.h"

class USystemsWorld;

UFUNCTION()
void MultiplayerServerApplyInputSystem(USystemsWorld& Systems);

UFUNCTION()
void MultiplayerClientApplyInputSystem(USystemsWorld& Systems);
