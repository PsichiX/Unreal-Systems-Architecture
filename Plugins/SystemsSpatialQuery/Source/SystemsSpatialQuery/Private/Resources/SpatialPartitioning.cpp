#include "SystemsSpatialQuery/Public/Resources/SpatialPartitioning.h"

#include "Systems/Public/SystemsWorld.h"

FVector FArea::Center() const
{
	return (this->Lower + this->Upper) * 0.5;
}

FVector FArea::Extents() const
{
	return Dimensions() * 0.5;
}

FVector FArea::Dimensions() const
{
	return this->Upper - this->Lower;
}

bool FArea::Contains(FVector Position) const
{
	return Position.X >= this->Lower.X && Position.X <= this->Upper.X && Position.Y >= this->Lower.Y &&
		Position.Y <= this->Upper.Y && Position.Z >= this->Lower.Z && Position.Z <= this->Upper.Z;
}

bool FArea::Overlaps(const FArea& Other) const
{
	return this->Lower.X <= Other.Upper.X && this->Upper.X >= Other.Lower.X && this->Lower.Y <= Other.Upper.Y &&
		this->Upper.Y >= Other.Lower.Y && this->Lower.Z <= Other.Upper.Z && this->Upper.Z >= Other.Lower.Z;
}

bool FArea::Overlaps(FVector Center, FVector::FReal Radius) const
{
	const auto SelfCenter = this->Center();
	const auto SelfExtents = this->Extents();
	const auto AxisDist = (Center - SelfCenter).GetAbs();
	if ((AxisDist.X > SelfExtents.X + Radius) || (AxisDist.Y > SelfExtents.Y + Radius) ||
		(AxisDist.Z > SelfExtents.Z + Radius))
	{
		return false;
	}
	if ((AxisDist.X < SelfExtents.X) || (AxisDist.Y < SelfExtents.Y) || (AxisDist.Z < SelfExtents.Z))
	{
		return true;
	}
	const auto CornerSquared = FVector::DistSquared(SelfExtents, AxisDist);
	return CornerSquared <= Radius * Radius;
}

ESpatialPreferredPlane FArea::SubdividePreferredAxis(ESpatialPreferredPlane PreferredPlane) const
{
	if (PreferredPlane == ESpatialPreferredPlane::None || PreferredPlane == ESpatialPreferredPlane::X ||
		PreferredPlane == ESpatialPreferredPlane::Y || PreferredPlane == ESpatialPreferredPlane::Z)
	{
		return ESpatialPreferredPlane::None;
	}
	const auto Dim = Dimensions();
	if (PreferredPlane == ESpatialPreferredPlane::XY)
	{
		if (Dim.X > Dim.Y)
		{
			return ESpatialPreferredPlane::X;
		}
		else
		{
			return ESpatialPreferredPlane::Y;
		}
	}
	else if (PreferredPlane == ESpatialPreferredPlane::YZ)
	{
		if (Dim.Y > Dim.Z)
		{
			return ESpatialPreferredPlane::Y;
		}
		else
		{
			return ESpatialPreferredPlane::Z;
		}
	}
	else if (PreferredPlane == ESpatialPreferredPlane::ZX)
	{
		if (Dim.Z > Dim.X)
		{
			return ESpatialPreferredPlane::Z;
		}
		else
		{
			return ESpatialPreferredPlane::X;
		}
	}
	else if (PreferredPlane == ESpatialPreferredPlane::Any)
	{
		if (Dim.X > Dim.Y && Dim.X > Dim.Z)
		{
			return ESpatialPreferredPlane::X;
		}
		else if (Dim.Y > Dim.Z && Dim.Y > Dim.X)
		{
			return ESpatialPreferredPlane::Y;
		}
		else
		{
			return ESpatialPreferredPlane::Z;
		}
	}
	return ESpatialPreferredPlane::None;
}

void FArea::Subdivide(TUniquePtr<FSpatialNode> (&Result)[2],
	FVector Center,
	uint32 Capacity,
	ESpatialPreferredPlane PreferredPlane) const
{
	const auto Axis = SubdividePreferredAxis(PreferredPlane);
	ensure(Axis != ESpatialPreferredPlane::None);

	if (Axis == ESpatialPreferredPlane::X)
	{
		Result[0] = MakeUnique<FSpatialNode>(FArea(FVector(this->Lower.X, this->Lower.Y, this->Lower.Z),
												 FVector(Center.X, this->Upper.Y, this->Upper.Z)),
			Capacity,
			PreferredPlane);
		Result[1] = MakeUnique<FSpatialNode>(FArea(FVector(Center.X, this->Lower.Y, this->Lower.Z),
												 FVector(this->Upper.X, this->Upper.Y, this->Upper.Z)),
			Capacity,
			PreferredPlane);
	}
	else if (Axis == ESpatialPreferredPlane::Y)
	{
		Result[0] = MakeUnique<FSpatialNode>(FArea(FVector(this->Lower.X, this->Lower.Y, this->Lower.Z),
												 FVector(this->Upper.X, Center.Y, this->Upper.Z)),
			Capacity,
			PreferredPlane);
		Result[1] = MakeUnique<FSpatialNode>(FArea(FVector(this->Lower.X, Center.Y, this->Lower.Z),
												 FVector(this->Upper.X, this->Upper.Y, this->Upper.Z)),
			Capacity,
			PreferredPlane);
	}
	else
	{
		Result[0] = MakeUnique<FSpatialNode>(FArea(FVector(this->Lower.X, this->Lower.Y, this->Lower.Z),
												 FVector(this->Upper.X, this->Upper.Y, Center.Z)),
			Capacity,
			PreferredPlane);
		Result[1] = MakeUnique<FSpatialNode>(FArea(FVector(this->Lower.X, this->Lower.Y, Center.Z),
												 FVector(this->Upper.X, this->Upper.Y, this->Upper.Z)),
			Capacity,
			PreferredPlane);
	}
}

FSpatialLeafs::FSpatialLeafs(TUniquePtr<FSpatialNode> (&&InLeafs)[2]) : Leafs()
{
	for (auto Index = 0; Index < 2; ++Index)
	{
		this->Leafs[Index] = MoveTemp(InLeafs[Index]);
	}
}

uint32 FSpatialNode::GetCount() const
{
	return this->Count;
}

bool FSpatialNode::Add(TObjectPtr<AActor> Actor, FArchetypeSignature InSignature)
{
	if (Actor == false)
	{
		return false;
	}
	if (this->Content.IsType<FSpatialActors>())
	{
		auto& Data = this->Content.Get<FSpatialActors>();
		if (Data.Actors.Num() < Data.Actors.Max())
		{
			this->Signature = this->Signature.Include(InSignature);
			Data.Actors.Add(Actor);
			++this->Count;
			return true;
		}
		else
		{
			auto Center = Actor->GetActorLocation();
			auto Num = 1;
			for (const auto& Current : Data.Actors)
			{
				if (Current)
				{
					Center += Current->GetActorLocation();
					++Num;
				}
			}
			Center /= Num;
			TUniquePtr<FSpatialNode> Leafs[2];
			this->Area.Subdivide(Leafs, Center, Data.Actors.Max(), this->PreferredPlane);
			while (Data.Actors.Num() > 0)
			{
				const auto Current = Data.Actors[0];
				Data.Actors.RemoveAtSwap(0, 1, false);
				if (Current == false)
				{
					continue;
				}
				const auto Location = Current->GetActorLocation();
				for (auto Index = 0; Index < 2; ++Index)
				{
					auto& Leaf = Leafs[Index];
					if (Leaf->Area.Contains(Location))
					{
						Leaf->Add(Current, InSignature);
						break;
					}
				}
			}
			this->Content.Set<FSpatialLeafs>(MoveTemp(Leafs));
		}
	}
	if (this->Content.IsType<FSpatialLeafs>())
	{
		auto& Data = this->Content.Get<FSpatialLeafs>();
		const auto Location = Actor->GetActorLocation();
		for (auto Index = 0; Index < 2; ++Index)
		{
			auto& Leaf = Data.Leafs[Index];
			if (Leaf->Area.Contains(Location))
			{
				if (Leaf->Add(Actor, InSignature))
				{
					this->Signature = this->Signature.Include(InSignature);
					++this->Count;
					return true;
				}
			}
		}
	}
	return false;
}

void FSpatialNode::GetActorsInArea(TSet<TObjectPtr<AActor>>& Result,
	const FArea& InArea,
	const TFunction<bool(AActor*)>& Validator) const
{
	if (this->Content.IsType<FSpatialActors>())
	{
		const auto& Data = this->Content.Get<FSpatialActors>();
		for (const auto& Actor : Data.Actors)
		{
			const auto Location = Actor->GetActorLocation();
			if (InArea.Contains(Location) && (!Validator || Validator(Actor)))
			{
				Result.Add(Actor);
			}
		}
	}
	else if (this->Content.IsType<FSpatialLeafs>())
	{
		const auto& Data = this->Content.Get<FSpatialLeafs>();
		for (auto Index = 0; Index < 2; ++Index)
		{
			const auto& Leaf = Data.Leafs[Index];
			if (Leaf->Area.Overlaps(InArea))
			{
				Leaf->GetActorsInArea(Result, InArea, Validator);
			}
		}
	}
}

void FSpatialNode::GetActorsInRadius(TSet<TObjectPtr<AActor>>& Result,
	FVector Center,
	FVector::FReal Radius,
	const TFunction<bool(AActor*)>& Validator) const
{
	if (this->Content.IsType<FSpatialActors>())
	{
		const auto& Data = this->Content.Get<FSpatialActors>();
		for (const auto& Actor : Data.Actors)
		{
			const auto Location = Actor->GetActorLocation();
			if (FVector::Distance(Center, Location) <= Radius && (!Validator || Validator(Actor)))
			{
				Result.Add(Actor);
			}
		}
	}
	else if (this->Content.IsType<FSpatialLeafs>())
	{
		const auto InArea = FArea(Center, Radius);
		const auto& Data = this->Content.Get<FSpatialLeafs>();
		for (auto Index = 0; Index < 2; ++Index)
		{
			const auto& Leaf = Data.Leafs[Index];
			if (Leaf->Area.Overlaps(InArea))
			{
				Leaf->GetActorsInRadius(Result, Center, Radius);
			}
		}
	}
}

void FSpatialNode::FindClosestActor(FVector Position,
	TOptional<FSpatialNodeClosest>& State,
	const TFunction<bool(AActor*)>& Validator) const
{
	if (State.IsSet() && this->Area.Overlaps(Position, State.GetValue().Distance) == false)
	{
		return;
	}
	if (this->Content.IsType<FSpatialActors>())
	{
		const auto& Data = this->Content.Get<FSpatialActors>();
		for (const auto& Actor : Data.Actors)
		{
			if (!Validator || Validator(Actor))
			{
				const auto Location = Actor->GetActorLocation();
				const auto Distance = FVector::Distance(Location, Position);
				if (State.IsSet())
				{
					if (Distance < State.GetValue().Distance)
					{
						State.Emplace(FSpatialNodeClosest(Actor, Distance));
					}
				}
				else
				{
					State = FSpatialNodeClosest(Actor, Distance);
				}
			}
		}
	}
	else if (this->Content.IsType<FSpatialLeafs>())
	{
		const auto& Data = this->Content.Get<FSpatialLeafs>();
		const auto& A = Data.Leafs[0];
		const auto& B = Data.Leafs[1];
		if (A->Area.Contains(Position))
		{
			A->FindClosestActor(Position, State, Validator);
			B->FindClosestActor(Position, State, Validator);
		}
		else
		{
			B->FindClosestActor(Position, State, Validator);
			A->FindClosestActor(Position, State, Validator);
		}
	}
}

const FSpatialNode* FSpatialNode::FindNode(FVector Position) const
{
	if (this->Area.Contains(Position) == false)
	{
		return nullptr;
	}
	if (this->Content.IsType<FSpatialLeafs>())
	{
		const auto& Data = this->Content.Get<FSpatialLeafs>();
		for (auto Index = 0; Index < 2; ++Index)
		{
			const auto& Leaf = Data.Leafs[Index];
			if (const auto* Result = Leaf->FindNode(Position))
			{
				return Result;
			}
		}
	}
	return this;
}

bool FSpatialNode::FindNodesPath(TArray<const FSpatialNode*>& Result, FVector Position) const
{
	if (this->Area.Contains(Position) == false)
	{
		return false;
	}
	Result.Add(this);
	if (this->Content.IsType<FSpatialLeafs>())
	{
		const auto& Data = this->Content.Get<FSpatialLeafs>();
		for (auto Index = 0; Index < 2; ++Index)
		{
			const auto& Leaf = Data.Leafs[Index];
			if (Leaf->FindNodesPath(Result, Position))
			{
				return true;
			}
		}
	}
	return false;
}

void FSpatialNode::ForEachArea(const TFunction<void(const FArea&, bool)> Callback) const
{
	if (this->Content.IsType<FSpatialActors>())
	{
		const auto& Data = this->Content.Get<FSpatialActors>();
		Callback(this->Area, Data.Actors.Num() > 0);
	}
	else if (this->Content.IsType<FSpatialLeafs>())
	{
		const auto& Data = this->Content.Get<FSpatialLeafs>();
		for (auto Index = 0; Index < 2; ++Index)
		{
			Data.Leafs[Index]->ForEachArea(Callback);
		}
	}
}

void USpatialPartitioning::Reset(const FArea& Area, uint32 Capacity, ESpatialPreferredPlane PreferredPlane)
{
	Capacity = FMath::Max(Capacity, (uint32) 1);
	this->Root = MakeUnique<FSpatialNode>(Area, Capacity, PreferredPlane);
}

uint32 USpatialPartitioning::GetCount() const
{
	return this->Root.IsValid() ? this->Root->GetCount() : 0;
}

bool USpatialPartitioning::Add(const USystemsWorld& Systems, TObjectPtr<AActor> Actor)
{
	if (this->Root.IsValid())
	{
		const auto Signature = Systems.ActorSignature(Actor);
		if (Signature.IsSet())
		{
			return this->Root->Add(Actor, Signature.GetValue());
		}
	}
	return false;
}

void USpatialPartitioning::GetActorsInArea(TSet<TObjectPtr<AActor>>& Result,
	const FArea& InArea,
	const TFunction<bool(AActor*)>& Validator) const
{
	if (this->Root.IsValid())
	{
		this->Root->GetActorsInArea(Result, InArea, Validator);
	}
}

void USpatialPartitioning::GetActorsInRadius(TSet<TObjectPtr<AActor>>& Result,
	FVector Center,
	FVector::FReal Radius,
	const TFunction<bool(AActor*)>& Validator) const
{
	if (this->Root.IsValid())
	{
		this->Root->GetActorsInRadius(Result, Center, Radius, Validator);
	}
}

TObjectPtr<AActor> USpatialPartitioning::FindClosestActor(FVector Position,
	const TFunction<bool(AActor*)>& Validator) const
{
	if (this->Root.IsValid())
	{
		TOptional<FSpatialNodeClosest> State = {};
		this->Root->FindClosestActor(Position, State, Validator);
		if (State.IsSet())
		{
			return State.GetValue().Actor;
		}
	}
	return {};
}

void USpatialPartitioning::ForEachArea(const TFunction<void(const FArea&, bool)> Callback) const
{
	if (this->Root.IsValid())
	{
		this->Root->ForEachArea(Callback);
	}
}
