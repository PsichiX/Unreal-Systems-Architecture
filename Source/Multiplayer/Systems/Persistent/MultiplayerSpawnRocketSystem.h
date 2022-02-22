#pragma once

#include "CoreMinimal.h"

class USystemsWorld;

UFUNCTION()
void MultiplayerSpawnRocketSystem(USystemsWorld& Systems);

UFUNCTION()
void MultiplayerSpawnRocketCooldownSystem(USystemsWorld& Systems);

UFUNCTION()
void MultiplayerExecuteRocketSpawnsSystem(USystemsWorld& Systems);
