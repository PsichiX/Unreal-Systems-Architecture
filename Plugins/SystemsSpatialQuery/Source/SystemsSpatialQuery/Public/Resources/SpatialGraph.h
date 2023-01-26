#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"

#include "SpatialGraph.generated.h"

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialGraphConnection
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> From = {};

	UPROPERTY()
	TObjectPtr<AActor> To = {};

	bool operator==(const FSpatialGraphConnection& Other) const;

	bool Equals(const FSpatialGraphConnection& Other) const;
};

uint32 GetTypeHash(const FSpatialGraphConnection& Value);

double SYSTEMSSPATIALQUERY_API DistanceSpatialGraphHeuristics(const AActor* Node,
	const AActor* Goal,
	const USpatialGraph* Graph);

double SYSTEMSSPATIALQUERY_API ManhattanSpatialGraphHeuristics(const AActor* Node,
	const AActor* Goal,
	const USpatialGraph* Graph);

USTRUCT()
struct SYSTEMSSPATIALQUERY_API FSpatialGraphPathFinderOpenItem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> Node = {};

	UPROPERTY()
	double Priority = 0;
};

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialGraphPathFinder
{
	GENERATED_BODY()

public:
	FSpatialGraphPathFinder()
	{
	}

	FSpatialGraphPathFinder(const USpatialGraph* InGraph,
		const TObjectPtr<AActor>& From,
		const TObjectPtr<AActor>& To,
		TFunction<double(const AActor* Node, const AActor* Goal, const USpatialGraph* Graph)> InHeuristics,
		uint32 NodesCountEstimate = 0);

	bool Step();

	TArray<TObjectPtr<AActor>> ReconstructPath() const;

	TArray<TObjectPtr<AActor>> Consume();

private:
	UPROPERTY()
	TArray<FSpatialGraphPathFinderOpenItem> Open = {};

	UPROPERTY()
	TMap<TObjectPtr<AActor>, double> Costs = {};

	UPROPERTY()
	TMap<TObjectPtr<AActor>, TObjectPtr<AActor>> Parents = {};

	UPROPERTY()
	TMap<TObjectPtr<AActor>, double> Distances = {};

	UPROPERTY()
	TObjectPtr<const USpatialGraph> Graph = {};

	UPROPERTY()
	TObjectPtr<AActor> Start = {};

	UPROPERTY()
	TObjectPtr<AActor> Goal = {};

	TFunction<double(const AActor* Node, const AActor* Goal, const USpatialGraph* Graph)> Heuristics = {};
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialGraph : public UObject
{
	GENERATED_BODY()

public:
	void Reset();

	void Add(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To, bool bBidirectional = true);

	bool HasNode(const TObjectPtr<AActor>& Actor) const;

	bool HasConnection(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To, bool bBidirectional = true) const;

	FSpatialGraphPathFinder FindPath(const TObjectPtr<AActor>& From,
		const TObjectPtr<AActor>& To,
		TFunction<double(const AActor* Node, const AActor* Goal, const USpatialGraph* Graph)> Heuristics,
		uint32 NodesCountEstimate = 0) const;

	auto NodesIter() const
	{
		return IterStdConst(this->Nodes);
	}

	auto ConnectionsIter() const
	{
		return IterStdConst(this->Connections)
			.Filter([](const auto& Item) { return Item.From && Item.To; })
			.Map<TTuple<TObjectPtr<AActor>, TObjectPtr<AActor>>>(
				[&](const auto& Item) { return MakeTuple(Item.From, Item.To); });
	}

	auto NeighborsIter(const TObjectPtr<AActor>& Actor) const
	{
		return IterStdConst(this->Connections)
			.Filter([&](const auto& Item) { return (Item.From == Actor || Item.To == Actor) && Item.From && Item.To; })
			.Map<TObjectPtr<AActor>>([&](const auto& Item) { return Item.From == Actor ? Item.To : Item.From; });
	}

	auto OutgoingNeighborsIter(const TObjectPtr<AActor>& From) const
	{
		return IterStdConst(this->Connections)
			.Filter([&](const auto& Item) { return Item.From == From && Item.From && Item.To; })
			.Map<TObjectPtr<AActor>>([&](const auto& Item) { return Item.To; });
	}

	auto IncomingNeighborsIter(const TObjectPtr<AActor>& To) const
	{
		return IterStdConst(this->Connections)
			.Filter([&](const auto& Item) { return Item.To == To && Item.From && Item.To; })
			.Map<TObjectPtr<AActor>>([&](const auto& Item) { return Item.From; });
	}

private:
	UPROPERTY()
	TSet<TObjectPtr<AActor>> Nodes = {};

	UPROPERTY()
	TSet<FSpatialGraphConnection> Connections = {};
};