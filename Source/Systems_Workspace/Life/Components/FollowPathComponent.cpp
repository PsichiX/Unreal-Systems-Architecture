#include "Systems_Workspace/Life/Components/FollowPathComponent.h"

#include "Systems/Public/Iterator.h"

struct Meta
{
	float DistanceSquared = 0;
	float LocationAlongPath = 0;
};

void UFollowPathComponent::SetPathFromPointsList(const TArray<FVector>& Points)
{
	this->PathSegments = IterStdConst(Points)
							 .Cast<FVector>()
							 .Views<2>()
							 .Map<FFollowPathSegment>(
								 [](const auto& View)
								 {
									 const auto StartPosition = FVector2D(View[0]);
									 const auto EndPosition = FVector2D(View[1]);
									 const auto Difference = EndPosition - StartPosition;
									 const auto Tangent = Difference.GetSafeNormal();
									 const auto Length = Difference.Size();

									 return FFollowPathSegment{
										 StartPosition, EndPosition, Tangent, Difference, Length};
								 })
							 .CollectArray();

	this->CachedPathLength = IterStdConst(this->PathSegments)
								 .Map<float>([](const auto& Segment) { return Segment.Length; })
								 .Sum(0.0f);
}

TOptional<FVector2D> UFollowPathComponent::SamplePositionAlongPath(float Location) const
{
	if (HasPath() == false)
	{
		return TOptional<FVector2D>();
	}

	if (Location <= 0)
	{
		return TOptional<FVector2D>(this->PathSegments[0].StartPosition);
	}

	if (Location >= this->CachedPathLength)
	{
		const auto Index = this->PathSegments.Num() - 1;
		return TOptional<FVector2D>(this->PathSegments[Index].EndPosition);
	}

	return IterStdConst(this->PathSegments)
		.FindMap<FVector2D>(
			[&](const auto& Segment)
			{
				if (Location >= 0 && Location <= Segment.Length)
				{
					return TOptional<FVector2D>(Segment.StartPosition + Segment.Tangent * Location);
				}
				Location -= Segment.Length;
				return TOptional<FVector2D>();
			});
}

TOptional<float> UFollowPathComponent::FindLocationAlongPath(const FVector2D& Position) const
{
	if (HasPath() == false)
	{
		return TOptional<float>();
	}

	float Total = 0;
	const auto Found =
		IterStdConst(this->PathSegments)
			.Map<Meta>(
				[&](const auto& Segment)
				{
					const auto Difference = Position - Segment.StartPosition;
					const auto Projection = FVector2D::DotProduct(Difference, Segment.Tangent);
					const auto ClampedProjection = FMath::Clamp(Projection, 0.0f, Segment.Length);
					const auto NearestPoint =
						Segment.StartPosition + Segment.Tangent * ClampedProjection;
					const auto DistanceSquared = FVector2D::DistSquared(Position, NearestPoint);
					const auto LocationAlongPath = Total + ClampedProjection;
					Total += Segment.Length;

					return Meta{DistanceSquared, LocationAlongPath};
				})
			.ComparedBy(
				[](const auto& A, const auto& B) { return A.DistanceSquared < B.DistanceSquared; });

	if (Found.IsSet())
	{
		return TOptional<float>(Found.GetValue().LocationAlongPath);
	}
	return TOptional<float>();
}

TOptional<FVector2D> UFollowPathComponent::ClosestPositionAlongPath(const FVector2D& Position) const
{
	if (const auto Location = FindLocationAlongPath(Position))
	{
		return TOptional<FVector2D>(SamplePositionAlongPath(Location.GetValue()));
	}
	return TOptional<FVector2D>();
}

bool UFollowPathComponent::HasReachedPathEnd(const FVector2D& Position, float Threshold) const
{
	if (const auto Location = FindLocationAlongPath(Position))
	{
		return this->CachedPathLength - Location.GetValue() < Threshold;
	}
	return false;
}
