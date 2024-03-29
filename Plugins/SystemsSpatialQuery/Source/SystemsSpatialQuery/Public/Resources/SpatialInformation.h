﻿#pragma once

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

	UPROPERTY(EditAnywhere)
	double DampingFactorBias = 0.0;
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
	friend class SYSTEMSSPATIALQUERY_API UDebugSpatialInformationSystem;

public:
	TOptional<double> Get(const TObjectPtr<AActor>& Actor, FName Id) const;

	double GetOrDefault(const TObjectPtr<AActor>& Actor, FName Id) const;

	void Set(const TObjectPtr<AActor>& Actor, FName Id, double Value);

	void Accumulate(const TObjectPtr<AActor>& Actor, FName Id, double RelativeValue);

	TOptional<double> GetDamping(const TObjectPtr<AActor>& Actor, FName Id) const;

	double GetDampingOrDefault(const TObjectPtr<AActor>& Actor, FName Id) const;

	void SetDamping(const TObjectPtr<AActor>& Actor, FName Id, double DampingFactorBias);

	void Unset(const TObjectPtr<AActor>& Actor, FName Id);

	void UnsetAll(const TObjectPtr<AActor>& Actor);

	void Reset();

	template <typename T>
	double SampleWeighted(FName Id, T ActorWeightTupleIter) const
	{
		auto Result = 0.0;
		for (const auto& Pair : ActorWeightTupleIter)
		{
			Result += GetOrDefault(Pair.Get<0>(), Id) * Pair.Get<1>();
		}
		return Result;
	}

	template <typename T>
	void ApplyWeighted(FName Id, double Value, T ActorWeightTupleIter)
	{
		for (const auto& Pair : ActorWeightTupleIter)
		{
			Accumulate(Pair.Get<0>(), Id, Value * Pair.Get<1>());
		}
	}

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