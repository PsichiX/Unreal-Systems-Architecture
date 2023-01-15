#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"

#include "SpatialInformation.generated.h"

class USpatialPartitioning;
class USystemsWorld;

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialInformationPoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, double> Values = {};
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialInformation : public UObject
{
	GENERATED_BODY()

public:
	TOptional<double> Get(const TObjectPtr<AActor>& Actor, FName Id) const;

	void Set(const TObjectPtr<AActor>& Actor, FName Id, double Value);

	void Unset(const TObjectPtr<AActor>& Actor, FName Id);

	void Clear(const TObjectPtr<AActor>& Actor);

	void Reset();

	double Sample(const FVector& Position,
		FName Id,
		const TArray<TObjectPtr<AActor>>& Actors,
		double Default = 0.0) const;

	auto Iter(const TObjectPtr<AActor>& Actor) const
	{
		static TMap<FName, double> EMPTY = {};
		auto& Result = EMPTY;
		const auto* Point = this->Points.Find(Actor);
		if (Point != nullptr)
		{
			Result = Point->Values;
		}
		return IterStdConst(Result);
	}

private:
	UPROPERTY()
	TMap<TObjectPtr<AActor>, FSpatialInformationPoint> Points = {};
};