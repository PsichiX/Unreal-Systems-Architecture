#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Components/SpatialComponent.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialPartitioning.h"

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

double USpatialInformation::SampleTriangle(const FVector& Position,
	FName Id,
	USystemsWorld& Systems,
	const USpatialPartitioning& Partitioning,
	double Default) const
{
	struct Meta
	{
		double Value = 0.0;
		AActor* Actor = nullptr;
	};

	const auto Values = Partitioning.Query<USpatialComponent>(Systems, Position)
							.FilterMap<Meta>(
								[&](const auto& QueryItem)
								{
									auto* Actor = QueryItem.Get<0>();
									const auto* Point = this->Points.Find(Actor);
									if (Point == nullptr)
									{
										return TOptional<Meta>();
									}
									const auto* Value = Point->Values.Find(Id);
									Meta Result = {};
									if (Value != nullptr)
									{
										Result.Value = *Value;
										Result.Actor = Actor;
									}
									return TOptional(Result);
								})
							.Take(3)
							.CollectArray();
	if (Values.Num() == 3)
	{
	}
	else if (Values.Num() == 2)
	{
		// return FMath::Lerp(Values[0].Value, Values[1].Value);
	}
	else if (Values.Num() == 1)
	{
		return Values[0].Value;
	}
	return Default;
}