#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"

#include "SpatialInformation.generated.h"

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialInformationValue
{
	GENERATED_BODY()

	UPROPERTY()
	double Value = 0.0;

	UPROPERTY()
	double Deviation = 0.0;
};

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialInformationPoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FSpatialInformationValue> Values = {};
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialInformation : public UObject
{
	GENERATED_BODY()

	friend class SYSTEMSSPATIALQUERY_API USpatialInformationSystem;

public:
	TOptional<double> Get(const TObjectPtr<AActor>& Actor, FName Id) const;

	double GetOrDefault(const TObjectPtr<AActor>& Actor, FName Id) const;

	void Set(const TObjectPtr<AActor>& Actor, FName Id, double Value);

	void Accumulate(const TObjectPtr<AActor>& Actor, FName Id, double RelativeValue);

	void Unset(const TObjectPtr<AActor>& Actor, FName Id);

	void UnsetAll(const TObjectPtr<AActor>& Actor);

	void Reset();

	double Sample(const FVector& Position, FName Id, const TSet<TObjectPtr<AActor>>& Actors) const;

	auto PointsIter() const
	{
		return IterStdConst(this->Points).Map<TObjectPtr<AActor>>([](const auto& Pair) { return Pair.Key; });
	}

	auto PointIter(const TObjectPtr<AActor>& Actor) const
	{
		static TMap<FName, FSpatialInformationValue> EMPTY = {};
		auto& Result = EMPTY;
		const auto* Point = this->Points.Find(Actor);
		if (Point != nullptr)
		{
			Result = Point->Values;
		}
		return IterStdConst(Result).Map<double>([](const auto& Pair) { return Pair.Value.Value; });
	}

private:
	UPROPERTY()
	TMap<TObjectPtr<AActor>, FSpatialInformationPoint> Points = {};
};