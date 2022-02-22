#include "Shared/Utils.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"

bool IsInVisionSpace(FVector Position,
	FVector Direction,
	FVector ObservedPosition,
	float PerceptionRange,
	float VisibilityConeAngleDegrees)
{
	const auto Diff = ObservedPosition - Position;
	const auto Dot = FVector::DotProduct(Direction.GetSafeNormal(), Diff.GetSafeNormal());
	const auto Threshold = FMath::Cos(FMath::DegreesToRadians(VisibilityConeAngleDegrees));
	return Diff.Size() <= PerceptionRange && Dot >= Threshold;
}

FSlateBrush MakeBrushFromSprite(UPaperSprite* Sprite)
{
	if (IsValid(Sprite))
	{
		const auto SpriteAtlasData = Sprite->GetSlateAtlasData();
		const auto SpriteSize = SpriteAtlasData.GetSourceDimensions();
		FSlateBrush Brush;

		Brush.SetResourceObject(Sprite);
		Brush.ImageSize = SpriteSize;
		return Brush;
	}

	return FSlateNoResource();
}
