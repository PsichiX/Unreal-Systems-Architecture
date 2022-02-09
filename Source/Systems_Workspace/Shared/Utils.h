#pragma once

#include "CoreMinimal.h"

struct FSlateBrush;
class UPaperSprite;

bool IsInVisionSpace(FVector Position,
	FVector Direction,
	FVector ObservedPosition,
	float PerceptionRange,
	float VisibilityConeAngleDegrees);

FSlateBrush MakeBrushFromSprite(UPaperSprite* Sprite);
