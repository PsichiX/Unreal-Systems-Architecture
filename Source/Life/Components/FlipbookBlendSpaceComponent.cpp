#include "Life/Components/FlipbookBlendSpaceComponent.h"

#include "Systems/Public/Iterator.h"

struct Meta
{
	float WeightedDistanceSquared = 0;
	UPaperFlipbook* Flipbook = nullptr;
};

UPaperFlipbook* UFlipbookBlendSpaceComponent::FindCurrentFlipbook(const FVector2D& Point) const
{
	auto* BlendSpace = this->Animations.Find(this->CurrentAnimation);
	if (BlendSpace == nullptr)
	{
		return nullptr;
	}

	const auto Found =
		IterStdConst(BlendSpace->Items)
			.Map<Meta>(
				[&](const auto& Item)
				{
					const auto DistanceSquared = FVector2D::DistSquared(Item.Point, Point);
					const auto WeightedDistanceSquared =
						Item.Weight > 0 ? DistanceSquared / Item.Weight : DistanceSquared;

					return Meta{WeightedDistanceSquared, Item.Flipbook};
				})
			.ComparedBy([](const auto& A, const auto& B)
				{ return A.WeightedDistanceSquared < B.WeightedDistanceSquared; });

	return Found.IsSet() ? Found.GetValue().Flipbook : nullptr;
}

float UFlipbookBlendSpaceComponent::FindCurrentPlayRateToSpeedFactor() const
{
	auto* BlendSpace = this->Animations.Find(this->CurrentAnimation);
	return BlendSpace != nullptr ? BlendSpace->PlayRateToSpeedFactor : 0;
}
