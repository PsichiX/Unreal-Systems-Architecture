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

double DistanceSpatialGraphHeuristics(const AActor* Node, const AActor* Goal, const USpatialGraph* Graph)
{
	return FVector::Distance(Node->GetActorLocation(), Goal->GetActorLocation());
}

double ManhattanSpatialGraphHeuristics(const AActor* Node, const AActor* Goal, const USpatialGraph* Graph)
{
	const auto A = Node->GetActorLocation();
	const auto B = Goal->GetActorLocation();
	return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y);
}

FSpatialGraphPathFinder::FSpatialGraphPathFinder(const USpatialGraph* InGraph,
	const TObjectPtr<AActor>& From,
	const TObjectPtr<AActor>& To,
	TFunction<double(const AActor* Node, const AActor* Goal, const USpatialGraph* Graph)> InHeuristics,
	uint32 NodesCountEstimate)
{
	if (IsValid(InGraph) == false || From == false || To == false)
	{
		return;
	}
	this->Graph = InGraph;
	this->Start = From;
	this->Goal = To;
	this->Heuristics = InHeuristics;
	this->Open.Reserve(NodesCountEstimate);
	this->Open.Push({this->Start, 0.0});
	this->Costs.Add(this->Start, 0.0);
	this->Distances.Add(this->Start, Heuristics(this->Start, this->Goal, this->Graph));
}

bool FSpatialGraphPathFinder::Step()
{
	if (this->Graph == false)
	{
		return false;
	}
	if (this->Open.IsEmpty())
	{
		return false;
	}
	this->Open.Sort([](const auto& A, const auto& B) { return A.Priority > B.Priority; });
	const auto Current = this->Open.Pop();
	if (Current.Node == this->Goal)
	{
		return false;
	}
	for (const auto& Neighbor : this->Graph->OutgoingNeighborsIter(Current.Node))
	{
		const auto Cost = this->Costs[Current.Node];
		if (this->Costs.Contains(Neighbor) == false || Cost < this->Costs[Neighbor])
		{
			this->Costs.Add(Neighbor) = Cost;
			const auto Priority = Cost + this->Heuristics(Neighbor, this->Goal, this->Graph);
			this->Open.Add({Neighbor, Priority});
			this->Parents.Add(Neighbor) = Current.Node;
		}
	}
	return true;
}

TArray<TObjectPtr<AActor>> FSpatialGraphPathFinder::ReconstructPath() const
{
	if (Parents.Contains(this->Goal) == false)
	{
		return {};
	}
	TArray<TObjectPtr<AActor>> Result = {};
	Result.Reserve(this->Parents.Num());
	auto Target = this->Goal;
	while (Target != this->Start)
	{
		Result.Add(Target);
		Target = this->Parents[Target];
	}
	Result.Add(this->Start);
	Algo::Reverse(Result);
	return Result;
}

TArray<TObjectPtr<AActor>> FSpatialGraphPathFinder::Consume()
{
	while (Step())
	{
	}
	return ReconstructPath();
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

FSpatialGraphPathFinder USpatialGraph::FindPath(const TObjectPtr<AActor>& From,
	const TObjectPtr<AActor>& To,
	TFunction<double(const AActor* Node, const AActor* Goal, const USpatialGraph* Graph)> Heuristics,
	uint32 NodesCountEstimate) const
{
	return FSpatialGraphPathFinder(this, From, To, Heuristics, NodesCountEstimate);
}