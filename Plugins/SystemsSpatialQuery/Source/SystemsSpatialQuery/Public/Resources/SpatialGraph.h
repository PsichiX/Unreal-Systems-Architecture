#pragma once

#include "CoreMinimal.h"

#include "Systems/Public/Iterator.h"

#include "SpatialGraph.generated.h"

USTRUCT(BlueprintType)
struct SYSTEMSSPATIALQUERY_API FSpatialGraphConnection
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 From = 0;

	UPROPERTY()
	uint32 To = 0;

	bool operator==(const FSpatialGraphConnection& Other) const;

	bool Equals(const FSpatialGraphConnection& Other) const;
};

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialGraph : public UObject
{
	GENERATED_BODY()

public:
	void Reset(uint32 NodesCapacity = 0, uint32 ConnectionsCapacity = 0);

	void Add(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To, bool bBidirectional = true);

	bool HasNode(const TObjectPtr<AActor>& Actor) const;

	bool HasConnection(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To, bool bBidirectional = true) const;

	auto NodesIter() const
	{
		return IterStdConst(this->Nodes);
	}

	auto ConnectionsIter() const
	{
		return IterStdConst(this->Connections)
			.Map<TTuple<TObjectPtr<AActor>, TObjectPtr<AActor>>>(
				[&](const auto& Item) { return MakeTuple(this->Nodes[Item.From], this->Nodes[Item.To]); })
			.Filter([](const auto& Pair) { return Pair.Get<0>() && Pair.Get<1>(); });
	}

	auto OutgoingNeighborsIter(const TObjectPtr<AActor>& From) const
	{
		checkf(this->Nodes.Contains(From), TEXT("`From` is not registered as spatial graph node!"));
		const auto Index = static_cast<uint32>(this->Nodes.Find(From));
		return IterStdConst(this->Connections)
			.Filter([&](const auto& Item) { return Item.From == Index; })
			.Map<TObjectPtr<AActor>>([&](const auto& Item) { return this->Nodes[Item.To]; })
			.Filter([](const auto& Actor) { return Actor != false; });
	}

	auto IncomingNeighborsIter(const TObjectPtr<AActor>& To) const
	{
		checkf(this->Nodes.Contains(To), TEXT("`To` is not registered as spatial graph node!"));
		const auto Index = static_cast<uint32>(this->Nodes.Find(To));
		return IterStdConst(this->Connections)
			.Filter([&](const auto& Item) { return Item.To == Index; })
			.Map<TObjectPtr<AActor>>([&](const auto& Item) { return this->Nodes[Item.From]; })
			.Filter([](const auto& Actor) { return Actor != false; });
	}

	auto NeighborsIter(const TObjectPtr<AActor>& Actor) const
	{
		checkf(this->Nodes.Contains(Actor), TEXT("`Actor` is not registered as spatial graph node!"));
		const auto Index = static_cast<uint32>(this->Nodes.Find(Actor));
		return IterStdConst(this->Connections)
			.Filter([&](const auto& Item) { return Item.From == Index || Item.To == Index; })
			.Map<TIterArray<2, uint32>>(
				[](const auto& Item) {
					return IterArray<2, uint32>({Item.From, Item.To});
				})
			.Flatten<uint32>()
			.Map<TObjectPtr<AActor>>([&](const auto Index) { return this->Nodes[Index]; })
			.Filter([](const auto& Actor) { return Actor != false; });
	}

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Nodes = {};

	UPROPERTY()
	TArray<FSpatialGraphConnection> Connections = {};
};
