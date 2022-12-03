#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/SystemsComponent.h"

#include "FollowPathComponent.generated.h"

USTRUCT()
struct FFollowPathSegment
{
	GENERATED_BODY()

	UPROPERTY()
	FVector2D StartPosition = FVector2D(0);

	UPROPERTY()
	FVector2D EndPosition = FVector2D(0);

	UPROPERTY()
	FVector2D Tangent = FVector2D(0);

	UPROPERTY()
	FVector2D Difference = FVector2D(0);

	UPROPERTY()
	double Length = 0;
};

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class LIFE_API UFollowPathComponent : public USystemsActorComponent
{
	GENERATED_BODY()

public:
	void SetPathFromPointsList(const TArray<FVector>& Points);

	void ClearPath()
	{
		this->PathSegments.Reset(0);
		this->CachedPathLength = 0;
	}

	bool HasPath() const
	{
		return this->PathSegments.Num() > 0;
	}

	float PathLength() const
	{
		return this->CachedPathLength;
	}

	TArrayView<const FFollowPathSegment> PathSegmentsView() const
	{
		return TArrayView<const FFollowPathSegment>(this->PathSegments);
	}

	TOptional<FVector2D> SamplePositionAlongPath(float Location) const;

	TOptional<float> FindLocationAlongPath(const FVector2D& Position) const;

	TOptional<FVector2D> ClosestPositionAlongPath(const FVector2D& Position) const;

	bool HasReachedPathEnd(const FVector2D& Position, float Threshold = 100) const;

private:
	UPROPERTY()
	TArray<FFollowPathSegment> PathSegments = {};

	UPROPERTY()
	float CachedPathLength = 0;
};
