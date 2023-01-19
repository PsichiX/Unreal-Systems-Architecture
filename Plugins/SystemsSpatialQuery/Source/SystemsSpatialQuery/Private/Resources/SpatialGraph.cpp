#include "SystemsSpatialQuery/Public/Resources/SpatialGraph.h"

bool FSpatialGraphConnection::operator==(const FSpatialGraphConnection& Other) const
{
	return Equals(Other);
}

bool FSpatialGraphConnection::Equals(const FSpatialGraphConnection& Other) const
{
	return this->From == Other.From && this->To == Other.To;
}

uint32 GetTypeHash(const FSpatialGraphConnection& Value)
{
	return HashCombine(GetTypeHash(Value.From), GetTypeHash(Value.To));
}

void USpatialGraph::Reset()
{
	this->Nodes.Reset();
	this->Connections.Reset();
}

void USpatialGraph::Add(const TObjectPtr<AActor>& From, const TObjectPtr<AActor>& To, bool bBidirectional)
{
	if (From == To)
	{
		return;
	}
	this->Nodes.Add(From);
	this->Nodes.Add(To);
	this->Connections.Add(FSpatialGraphConnection{From, To});
	if (bBidirectional)
	{
		this->Connections.Add(FSpatialGraphConnection{To, From});
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
	if (this->Connections.Contains(FSpatialGraphConnection{From, To}))
	{
		return true;
	}
	if (bBidirectional && this->Connections.Contains(FSpatialGraphConnection{To, From}))
	{
		return true;
	}
	return false;
}