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
			return Value->Value;
		}
	}
	return {};
}

double USpatialInformation::GetOrDefault(const TObjectPtr<AActor>& Actor, FName Id) const
{
	const auto Value = Get(Actor, Id);
	return Value.IsSet() ? Value.GetValue() : 0.0;
}

void USpatialInformation::Set(const TObjectPtr<AActor>& Actor, FName Id, double Value)
{
	this->Points.FindOrAdd(Actor).Values.FindOrAdd(Id).Value = Value;
}

void USpatialInformation::Accumulate(const TObjectPtr<AActor>& Actor, FName Id, double RelativeValue)
{
	this->Points.FindOrAdd(Actor).Values.FindOrAdd(Id).Value += RelativeValue;
}

TOptional<double> USpatialInformation::GetDamping(const TObjectPtr<AActor>& Actor, FName Id) const
{
	const auto* Point = this->Points.Find(Actor);
	if (Point != nullptr)
	{
		const auto* Value = Point->Values.Find(Id);
		if (Value != nullptr)
		{
			return Value->DampingFactorBias;
		}
	}
	return {};
}

double USpatialInformation::GetDampingOrDefault(const TObjectPtr<AActor>& Actor, FName Id) const
{
	const auto Value = GetDamping(Actor, Id);
	return Value.IsSet() ? Value.GetValue() : 0.0;
}

void USpatialInformation::SetDamping(const TObjectPtr<AActor>& Actor, FName Id, double DampingFactorBias)
{
	this->Points.FindOrAdd(Actor).Values.FindOrAdd(Id).DampingFactorBias = DampingFactorBias;
}

void USpatialInformation::Unset(const TObjectPtr<AActor>& Actor, FName Id)
{
	auto* Point = this->Points.Find(Actor);
	if (Point != nullptr)
	{
		Point->Values.Remove(Id);
	}
}

void USpatialInformation::UnsetAll(const TObjectPtr<AActor>& Actor)
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