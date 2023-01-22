#include "SystemsSpatialQuery/Public/Utils/SceneVoxelTree.h"

FSceneVoxelCells::FSceneVoxelCells(TUniquePtr<FSceneVoxelNode> (&&InCells)[8])
{
	for (auto Index = 0; Index < 8; ++Index)
	{
		this->Cells[Index] = MoveTemp(InCells[Index]);
	}
}

TUniquePtr<FSceneVoxelNode> FSceneVoxelNode::Make(FBox InBoundingBox, double InMinSize)
{
	return MakeUnique<FSceneVoxelNode>(InBoundingBox, InMinSize);
}

void FSceneVoxelNode::Add(TObjectPtr<UPrimitiveComponent> Primitive)
{
	if (Primitive == false)
	{
		return;
	}
	const auto PrimitiveBox = Primitive->Bounds.GetBox();
	if (PrimitiveBox.Intersect(this->BoundingBox) == false)
	{
		return;
	}
	if (this->Content.IsType<FSceneVoxelEmpty>())
	{
		const auto Extent = this->BoundingBox.GetExtent();
		if (Extent.X < this->MinSize || Extent.Y < this->MinSize || Extent.Z < this->MinSize)
		{
			this->Content.Set<FSceneVoxelPrimitives>({});
		}
		else
		{
			const auto Min = this->BoundingBox.Min;
			const auto Max = this->BoundingBox.Max;
			const auto Center = this->BoundingBox.GetCenter();
			TUniquePtr<FSceneVoxelNode> Cells[8] = {
				Make(FBox(FVector(Min.X, Min.Y, Min.Z), FVector(Center.X, Center.Y, Center.Z)), this->MinSize),
				Make(FBox(FVector(Center.X, Min.Y, Min.Z), FVector(Max.X, Center.Y, Center.Z)), this->MinSize),
				Make(FBox(FVector(Center.X, Center.Y, Min.Z), FVector(Max.X, Max.Y, Center.Z)), this->MinSize),
				Make(FBox(FVector(Min.X, Center.Y, Min.Z), FVector(Min.X, Max.Y, Center.Z)), this->MinSize),
				Make(FBox(FVector(Min.X, Min.Y, Center.Z), FVector(Center.X, Center.Y, Max.Z)), this->MinSize),
				Make(FBox(FVector(Center.X, Min.Y, Center.Z), FVector(Max.X, Center.Y, Max.Z)), this->MinSize),
				Make(FBox(FVector(Center.X, Center.Y, Center.Z), FVector(Max.X, Max.Y, Max.Z)), this->MinSize),
				Make(FBox(FVector(Min.X, Center.Y, Center.Z), FVector(Min.X, Max.Y, Max.Z)), this->MinSize),
			};
			this->Content.Set<FSceneVoxelCells>(MoveTemp(Cells));
		}
	}
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			const auto& Cell = Data.Cells[Index];
			Cell->Add(Primitive);
			if (Cell->ClosestPointOnSurface.IsSet())
			{
				const auto Distance =
					FVector::Distance(this->BoundingBox.GetCenter(), Cell->ClosestPointOnSurface.GetValue());
				if (this->ClosestDistanceToSurface.IsSet() == false ||
					Distance < this->ClosestDistanceToSurface.GetValue())
				{
					this->ClosestDistanceToSurface = Distance;
					this->ClosestPointOnSurface = Cell->ClosestPointOnSurface;
				}
			}
		}
	}
	else if (this->Content.IsType<FSceneVoxelPrimitives>())
	{
		auto& Data = this->Content.Get<FSceneVoxelPrimitives>();
		if (Data.Components.Contains(Primitive) == false)
		{
			FVector Point = {};
			const auto Status = Primitive->GetClosestPointOnCollision(this->BoundingBox.GetCenter(), Point);
			if (Status > 0.0)
			{
				const auto Distance = FVector::Distance(this->BoundingBox.GetCenter(), Point);
				if (this->ClosestDistanceToSurface.IsSet() == false ||
					Distance < this->ClosestDistanceToSurface.GetValue())
				{
					this->ClosestDistanceToSurface = Distance;
					this->ClosestPointOnSurface = Point;
				}
				Data.Components.Add(Primitive);
			}
		}
	}
}

void FSceneVoxelNode::ForEachNode(const TFunctionRef<void(const FSceneVoxelNode& Node)> Callback,
	ESceneVoxelQueryOptions Options) const
{
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		if (Options != ESceneVoxelQueryOptions::PrimitivesOnly)
		{
			Callback(*this);
		}
		auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			Data.Cells[Index]->ForEachNode(Callback);
		}
	}
	else if (this->Content.IsType<FSceneVoxelPrimitives>())
	{
		Callback(*this);
	}
}

const FSceneVoxelNode* FSceneVoxelNode::FindNode(FVector Position, ESceneVoxelQueryOptions Options) const
{
	if (this->BoundingBox.IsInsideOrOn(Position) == false)
	{
		return nullptr;
	}
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			const auto& Cell = Data.Cells[Index];
			if (const auto* Node = Cell->FindNode(Position, Options))
			{
				return Node;
			}
		}
	}
	if (Options == ESceneVoxelQueryOptions::PrimitivesOnly && this->Content.IsType<FSceneVoxelPrimitives>() == false)
	{
		return nullptr;
	}
	if (Options == ESceneVoxelQueryOptions::Occupied && this->Content.IsType<FSceneVoxelEmpty>())
	{
		return nullptr;
	}
	return this;
}

TOptional<FVector> FSceneVoxelNode::FindClosestPointOnSurface(FVector Position) const
{
	if (this->BoundingBox.IsInsideOrOn(Position) == false)
	{
		Position = this->BoundingBox.GetClosestPointTo(Position);
	}
	const auto* Node = FindNode(Position);
	if (Node != nullptr)
	{
		return Node->ClosestPointOnSurface;
	}
	return {};
}

TOptional<FVector> FSceneVoxelNode::GetClosestPointOnSurface() const
{
	return this->ClosestPointOnSurface;
}

TOptional<double> FSceneVoxelNode::GetClosestDistanceToSurface() const
{
	return this->ClosestDistanceToSurface;
}

bool FSceneVoxelNode::IsOccupied() const
{
	return this->Content.IsType<FSceneVoxelEmpty>() == false;
}

void FSceneVoxelNode::GetAllPrimitives(TSet<TObjectPtr<UPrimitiveComponent>>& Result) const
{
	if (this->Content.IsType<FSceneVoxelCells>())
	{
		const auto& Data = this->Content.Get<FSceneVoxelCells>();
		for (auto Index = 0; Index < 8; ++Index)
		{
			Data.Cells[Index]->GetAllPrimitives(Result);
		}
	}
	else if (this->Content.IsType<FSceneVoxelPrimitives>())
	{
		const auto& Data = this->Content.Get<FSceneVoxelPrimitives>();
		for (const auto& Primitive : Data.Components)
		{
			Result.Add(Primitive);
		}
	}
}