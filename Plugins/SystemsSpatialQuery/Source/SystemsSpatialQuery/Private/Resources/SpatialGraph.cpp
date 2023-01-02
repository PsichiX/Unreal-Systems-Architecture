#include "SystemsSpatialQuery/Public/Resources/SpatialGraph.h"

bool FSpatialGraphConnection::operator==(const FSpatialGraphConnection& Other) const
{
	return Equals(Other);
}

bool FSpatialGraphConnection::Equals(const FSpatialGraphConnection& Other) const
{
	return this->From == Other.From && this->To == Other.To;
}

void USpatialGraph::Reset(uint32 NodesCapacity, uint32 ConnectionsCapacity)
{
	this->Nodes.Reset(NodesCapacity);
	this->Connections.Reset(ConnectionsCapacity);
}

void USpatialGraph::Add(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To, bool bBidirectional)
{
	if (From == To)
	{
		return;
	}
	const auto FromIndex = static_cast<uint32>(this->Nodes.AddUnique(From));
	const auto ToIndex = static_cast<uint32>(this->Nodes.AddUnique(To));
	this->Connections.AddUnique(FSpatialGraphConnection{FromIndex, ToIndex});
	if (bBidirectional)
	{
		this->Connections.AddUnique(FSpatialGraphConnection{ToIndex, FromIndex});
	}
}

bool USpatialGraph::HasNode(const TObjectPtr<AActor>& Actor) const
{
	return this->Nodes.Contains(Actor);
}

bool USpatialGraph::HasConnection(const TObjectPtr<AActor>& From,
	const TObjectPtr<AActor>& To,
	bool bBidirectional) const
{
	if (From == To)
	{
		return false;
	}
	const auto FoundFrom = this->Nodes.Find(From);
	const auto FoundTo = this->Nodes.Find(To);
	if (FoundFrom < 0 || FoundTo < 0)
	{
		return false;
	}
	const auto FromIndex = static_cast<uint32>(FoundFrom);
	const auto ToIndex = static_cast<uint32>(FoundTo);
	if (this->Connections.Contains(FSpatialGraphConnection{FromIndex, ToIndex}))
	{
		return true;
	}
	if (bBidirectional && this->Connections.Contains(FSpatialGraphConnection{ToIndex, FromIndex}))
	{
		return true;
	}
	return false;
}
