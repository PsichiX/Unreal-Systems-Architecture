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

double UDistanceSpatialGraphHeuristics::Calculate(const AActor* Node,
	const AActor* Goal,
	const USpatialGraph* Graph) const
{
	return FVector::Distance(Node->GetActorLocation(), Goal->GetActorLocation());
}
double UManhattanSpatialGraphHeuristics::Calculate(const AActor* Node,
	const AActor* Goal,
	const USpatialGraph* Graph) const
{
	const auto A = Node->GetActorLocation();
	const auto B = Goal->GetActorLocation();
	return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y);
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

TArray<TObjectPtr<AActor>> USpatialGraph::FindPath(const TObjectPtr<AActor>& From,
	const TObjectPtr<AActor>& To,
	uint32 NodesCountEstimate) const
{
	struct Meta
	{
		TObjectPtr<AActor> Node = {};
		double Priority = 0;
	};

	if (this->Heuristics == false || IsValid(From) == false || IsValid(To) == false ||
		this->Nodes.Contains(From) == false || this->Nodes.Contains(To) == false)
	{
		return {};
	}
	if (From == To)
	{
		return {From, To};
	}
	TArray<Meta> Open = {};
	Open.Reserve(NodesCountEstimate);
	Open.Push({From, 0.0});
	TMap<TObjectPtr<AActor>, double> Costs = {};
	Costs.Add(From, 0.0);
	TMap<TObjectPtr<AActor>, TObjectPtr<AActor>> Parents = {};
	TMap<TObjectPtr<AActor>, double> Distances = {};
	Distances.Add(From, this->Heuristics->Calculate(From, To, this));
	while (Open.IsEmpty() == false)
	{
		Open.Sort([](const auto& A, const auto& B) { return A.Priority > B.Priority; });
		const auto Current = Open.Pop();
		if (Current.Node == To)
		{
			break;
		}
		for (const auto& Neighbor : OutgoingNeighborsIter(Current.Node))
		{
			const auto Cost = Costs[Current.Node];
			if (Costs.Contains(Neighbor) == false || Cost < Costs[Neighbor])
			{
				Costs[Neighbor] = Cost;
				const auto Priority = Cost + this->Heuristics->Calculate(Neighbor, To, this);
				Open.Add({Neighbor, Priority});
				Parents[Neighbor] = Current.Node;
			}
		}
	}
	if (Parents.Contains(To) == false)
	{
		return {};
	}
	TArray<TObjectPtr<AActor>> Result = {};
	Result.Reserve(Parents.Num());
	auto Target = To;
	while (Target != From)
	{
		Result.Add(Target);
		Target = Parents[Target];
	}
	Result.Add(From);
	Algo::Reverse(Result);
	return Result;
}