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

UCLASS(BlueprintType)
class SYSTEMSSPATIALQUERY_API USpatialGraph : public UObject
{
	GENERATED_BODY()

public:
	void Reset();

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