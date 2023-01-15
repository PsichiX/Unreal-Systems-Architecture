#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

#include "SystemsSpatialQuery/Public/Components/SpatialComponent.h"

TOptional<double> USpatialInformation::Get(const TObjectPtr<AActor>& Actor, FName Id) const
{
	const auto* Point = this->Points.Find(Actor);
	if (Point != nullptr)
	{
		const auto* Value = Point->Values.Find(Id);
		if (Value != nullptr)
		{
			return *Value;
		}
	}
	return {};
}

void USpatialInformation::Set(const TObjectPtr<AActor>& Actor, FName Id, double Value)
{
	auto& Point = this->Points.FindOrAdd(Actor);
	Point.Values.Add(Id, Value);
}

void USpatialInformation::Unset(const TObjectPtr<AActor>& Actor, FName Id)
{
	auto* Point = this->Points.Find(Actor);
	if (Point != nullptr)
	{
		Point->Values.Remove(Id);
	}
}

void USpatialInformation::Clear(const TObjectPtr<AActor>& Actor)
{
	auto* Point = this->Points.Find(Actor);
	if (Point != nullptr)
	{
		Point->Values.Reset();
	}
}

void USpatialInformation::Reset()
{
	this->Points.Reset();
}

double USpatialInformation::Sample(const FVector& Position,
	FName Id,
	const TArray<TObjectPtr<AActor>>& Actors,
	double Default) const
{
	struct FMetaInformation
	{
		double Value = 0.0;
		double Distance = 0.0;
	};

	TArray<FMetaInformation> Values = {};
	Values.Reserve(Actors.Num());
	for (const auto& Actor : Actors)
	{
		const auto Location = Actor->GetActorLocation();
		const auto Distance = FVector::Distance(Position, Location);
		const auto ValueOpt = Get(Actor, Id);
		const auto Value = ValueOpt.IsSet() ? ValueOpt.GetValue() : Default;
		Values.Add({Value, Distance});
	}
	if (Values.Num() <= 0)
	{
		return Default;
	}
	if (Values.Num() == 1)
	{
		return Values[0].Value;
	}
	const auto MaxDistance = IterStdConst(Values).Fold<double>(
		0.0, [](const auto Accum, const auto& Meta) { return FMath::Max(Accum, Meta.Distance); });
	auto TotalWeight = 0.0;
	auto Result = 0.0;
	for (const auto& Meta : Values)
	{
		const auto Weight = FMath::Clamp(1.0 - (Meta.Distance / MaxDistance), 0.0, 1.0);
		TotalWeight += Weight;
		Result += Meta.Value * Weight;
	}
	return Result / TotalWeight;
}