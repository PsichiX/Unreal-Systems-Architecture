#pragma once

#include "CoreMinimal.h"

struct FSlateBrush;
class UPaperSprite;

bool SHARED_API IsInVisionSpace(FVector Position,
	FVector Direction,
	FVector ObservedPosition,
	float PerceptionRange,
	float VisibilityConeAngleDegrees);

FSlateBrush SHARED_API MakeBrushFromSprite(UPaperSprite* Sprite);
